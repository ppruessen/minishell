/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 14:43:18 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/04 11:34:10 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	print_cmd(t_var *var, int i)
{
	int	j;

	printf("----------------------COMMANDS[%i]------------------------\n", i);
	printf("var->cmds[%d]->inbuilt = %d\n", i, var->cmds[i]->inbuilt);
	printf("var->cmds[%d]->read_from_file = %d\n", i, var->cmds[i]->read_from_file);
	printf("var->cmds[%d]->write_to_file = %d\n", i, var->cmds[i]->write_to_file);
	printf("var->cmds[%d]->read_from_pipe = %d\n", i, var->cmds[i]->read_from_pipe);
	printf("var->cmds[%d]->write_to_pipe = %d\n", i, var->cmds[i]->write_to_pipe);
	printf("var->pipes = %d\n", var->pipes);
	j = 0;
	while(var->cmds[i]->cmd[j] != NULL)
	{
		printf("CMD[%d]:\t\t %s\n", j, var->cmds[i]->cmd[j]);
		printf("CMD_ESC[%d]:\t %s\n", j, var->cmds[i]->cmd_esc[j]);
		j++;
	}
	printf("--------------------------------------------------------\n");
}

/* Function creates an absolute path from a relativ path */
char	*create_rel_path(char *cmd_path)
{
	char	pwd[500];
	char	*pwd_temp;
	char	*cmd_path_temp;

	ft_bzero(pwd, 500);
	getcwd(pwd, 500);
	pwd_temp = ft_strdup(pwd);
	cmd_path_temp = ft_strdup(cmd_path);
	free (cmd_path);
	cmd_path = NULL;
	pwd_temp = ft_strjoin_char(pwd_temp, '/');
	cmd_path = ft_strjoin(pwd_temp, cmd_path_temp);
	free (pwd_temp);
	pwd_temp = NULL;
	free (cmd_path_temp);
	cmd_path_temp = NULL;
	return (cmd_path);
}

/* Function to get the PATH Variable from the env_list */
static void	get_path(t_var *var)
{
	t_list	*list;

	list = var->env_list;
	while (list)
	{
		if (!ft_strncmp("PATH=", list->content, ft_strlen("PATH=")))
		{
			var->path = ft_strnstr(list->content, 
				"PATH=", ft_strlen(list->content));
			var->path = ft_strtrim(var->path, "PATH=");
			break ;
		}
		list = list->next;
	}
	return ;
}

/* Function thas generates a directory list */
static void	gen_dir_list(t_var *var)
{
	int		i;
	char	*temp;

	i = 0;
	var->dir_list = ft_split(var->path, ':');
	free (var->path);
	var->path = NULL;
	while (var->dir_list[i] != NULL)
	{
		temp = ft_strjoin(var->dir_list[i], "/");
		free (var->dir_list[i]);
		var->dir_list[i] = NULL;
		var->dir_list[i] = temp;
		i++;
	}
}

/* Checks if the command is found within the path directories. */
/* Returns the correct path or the cmd if path not found. */
char	*check_and_set_path(char *cmd, t_var *var)
{
	char	*valid_cmd_path;
	int		i;

	get_path(var);
	gen_dir_list(var);
	i = 0;
	while (var->dir_list[i] != NULL)
	{
		if (cmd[0] == '\0')
			break ;
		valid_cmd_path = ft_strjoin(var->dir_list[i], cmd);
		if (access(valid_cmd_path, X_OK) == 0)
		{
			if (debug_mode < -3)
				printf("Execute_cmds.c/111: Valid cmd path in check and set path: |%s|\n", valid_cmd_path);
			i = 0;
			while (var->dir_list[i] != NULL)
			{
				free (var->dir_list[i]);
				var->dir_list[i] = NULL;
				i++;
			}
			free (var->dir_list);
			var->dir_list = NULL;
			free (cmd);
			cmd = NULL;
			return (valid_cmd_path);
		}
		i++;
	}
	return (cmd);
}

/* Sets the cmd path to unbuilt functions */
char	*set_inbuilt_path(t_cmd *cmd, t_var *var)
{
	if (cmd->inbuilt == ECHO || cmd->inbuilt == ENV || cmd->inbuilt == PWD)
	{
		free(cmd->cmd[0]);
		cmd->cmd[0] = NULL;
	}
	if (cmd->inbuilt == ECHO)
		return(ft_strjoin(var->pwd, "/incl/inbuilts_progs/echo"));
	else if (cmd->inbuilt == ENV)
		return(ft_strjoin(var->pwd, "/incl/inbuilts_progs/env"));
	else if (cmd->inbuilt == PWD)
		return(ft_strjoin(var->pwd, "/incl/inbuilts_progs/pwd"));
	return (cmd->cmd[0]);
}

/* Sets the cmd path */
void	set_cmd_path(t_cmd *cmd, t_var *var)
{
	int	i;

	i = 0;
	while (cmd->cmd[i] && (cmd->cmd_esc[i][0] == 'F' || cmd->cmd_esc[i][0] == 'W')) //HIER?
		i++;
	if (cmd->cmd[i] && cmd->inbuilt == 0)
	{
		if (cmd->cmd[i][0] == '.')
			cmd->cmd[i] = create_rel_path(cmd->cmd[i]);
		else if (cmd->cmd[i][0] == '/')
			return ;
		else
			cmd->cmd[i] = check_and_set_path(cmd->cmd[i], var);
	}
	else if (cmd->cmd[i])
		cmd->cmd[i] = set_inbuilt_path(cmd, var);
	if (debug_mode < -3)
		printf("Execute_cmds.c/154: cmd->cmd[0] in set cmd path: |%s|\n", cmd->cmd[i]);
}

/* Function that opens the file descriptor to the read file */
void	open_read_from_file(t_cmd *cmd)
{
	if (cmd->read_from_file == FROM_FILE)
	{
		cmd->fd_to_read = open(cmd->filename_to_read, O_RDONLY);
		dup2(cmd->fd_to_read, STDIN_FILENO);
		close(cmd->fd_to_read);
	}
	if (cmd->read_from_file == FROM_HERE_DOC)
	{
		cmd->fd_to_read = open(".bonus_tmp", O_RDWR | O_CREAT | O_TRUNC, 0644);
		handle_here_doc(cmd);
		cmd->fd_to_read = open(".bonus_tmp", O_RDONLY);
		dup2(cmd->fd_to_read, STDIN_FILENO);
		close(cmd->fd_to_read);
	}
}

/* Function that opens the file descriptor to the write file */
void	open_write_to_file(t_cmd *cmd)
{
	if (cmd->write_to_file == INTO_FILE)
	{
		unlink(cmd->filename_to_write);
		cmd->fd_to_write = open(cmd->filename_to_write, O_WRONLY | O_CREAT, 0777);
		dup2(cmd->fd_to_write, STDOUT_FILENO);
		close(cmd->fd_to_write);
	}
	else if (cmd->write_to_file == INTO_FILE_APPEND)
	{
		cmd->fd_to_write = open(cmd->filename_to_write, O_RDWR | O_CREAT | O_APPEND, 0644);
		dup2(cmd->fd_to_write, STDOUT_FILENO);
		close(cmd->fd_to_write);
	}
}

/* Function that sets the pipes and filedescriptors */
void	open_redirections(t_cmd *cmd)
{
	if (cmd->read_from_file != 0)
		open_read_from_file(cmd);
	if (cmd->write_to_file != 0)
		open_write_to_file(cmd);
}

/* Function that closes all READ pipes in child process */
void	close_all_read_pipes(t_var *var)
{
	int	j;

	j = 0;
	while (j < var->pipes)
	{
		if (close(var->fd[j][READ]) == -1)
		{
			printf("Unable to close var->fd[%d][READ]\n", j);
		}
		j++;
	}
}

/* Function set the pipes in the child process */
void	set_pipes_in_child(t_var *var, t_cmd *cmd, int i)
{
	if (cmd->write_to_pipe == 1 && cmd->read_from_pipe == 1)
	{
		if (dup2(var->fd[i - 1][READ], STDIN_FILENO) == -1)
			printf("Unable to dup2 var->fd[%i][READ]\n", i - 1);
		if (dup2(var->fd[i][WRITE], STDOUT_FILENO) == -1)
			printf("Unable to dup2 var->fd[%i][WRITE]\n", i);
		close_all_read_pipes(var);
	}
	else if (cmd->write_to_pipe == 1)
	{
		if (dup2(var->fd[i][WRITE], STDOUT_FILENO) == -1)
			printf("Unable to dup2 var->fd[%i][WRITE]\n", i);
		close_all_read_pipes(var);
	}
	else if (cmd->read_from_pipe == 1)
	{
		if (dup2(var->fd[i - 1][READ], STDIN_FILENO) == -1)
			printf("Unable to dup2 var->fd[%i][READ]\n", i - 1);
		close_all_read_pipes(var);
	}
}

/* Funnction to close all pipes in parent process */
void	close_pipes_in_parent(t_var *var, int i)
{
	if (var->cmds[i]->write_to_pipe == 1)
		close(var->fd[i][WRITE]);
	else
		close(var->fd[i - 1][READ]);
}

/* Executes the cmd */
void	execute_cmd(t_cmd *cmd, t_var *var, int i)
{
		int	j;
		int	k;
		char **execve_input;

		if (var->pipes > 0)
			set_pipes_in_child(var, cmd, i);
		open_redirections(cmd);
		int input_members = 0;
		j = 0;
		while (cmd->cmd[j] != NULL)
		{
			if (cmd->cmd_esc[j][0] != 'F' && (cmd->cmd_esc[j][0] != 'W'))
				input_members++;
			j++;
		}
		execve_input = (char **) malloc(((input_members + 1) * sizeof(char*)) + 1);
		if (execve_input == NULL)
			return ;
		execve_input[input_members] = NULL;
		j = 0;
		k = 0;
		while (cmd->cmd[j])
		{
			if (cmd->cmd_esc[j][0] == 'F' || cmd->cmd_esc[j][0] == 'W')
				;
			else
			{
				execve_input[k] = ft_strdup(cmd->cmd[j]);
				k++;
			}
			j++;
		}
		k = 0;
		while (execve_input[k] != NULL)
		{
			if (debug_mode < -3)
				printf("Execute_cmds.c/297: Das wird ins excecve gegeben: |%s|\n", execve_input[k]);
			k++;
		}
		if (execve(*execve_input, execve_input, var->env) == -1)
		{
			print_cmd_error(var, CMD_NOT_FOUND, execve_input[0]);
//			clean_up(var);
			exit(127);
		}
		else
			printf("ANDERER FEHLER IM EXECVE\n");
}

/* Function to open the filedescriptors */
void	open_filedescriptors(t_var *var)
{
	int	i;

	var->fd = malloc((var->pipes) * sizeof(var->fd[2]));
	i = 0;
	while (i < var->pipes)
	{
		if (pipe(var->fd[i]) == -1)
			printf("Could not open pipe[%d].\n", i);
		i++;
	}
}

/* Function that waits for the child process */
void	wait_function(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failed");
		return ;
	}
	if (WIFEXITED(status))
	{
		const int es = WEXITSTATUS(status);
		if (debug_mode < -3)
			printf("Execute_cmds.c/337: exit status was %d\n", es);
		if (es == 255)
			g_status = 127;
		g_status = es;
	}
}

/* Function that calls execve or inbuilts functions and sets the correct pipes
// and/or files to read and write to */
void	execute_cmds(t_var *var)
{
	int		i;
	pid_t	pid;

	if (var->pipes > 0)
		open_filedescriptors(var);
	i = 0;
	if (var->cmds == NULL)
		return ;
	while (var->cmds[i] != NULL)
	{
		if (debug_mode < -3)
			printf("Execute_cmds.c/361: before finding inbuilts\n var->cmds[i]->cmd[0]: %s\n", var->cmds[i]->cmd[0]);
		find_inbuilts(var, var->cmds[i], var->cmds[i]->cmd[0]);
		if (debug_mode < -3)
			printf("Execute_cmds.c/364: after finding inbuilts\n var->cmds[i]->inbuilt = %d\n", var->cmds[i]->inbuilt);
		if (var->cmds[i]->write_to_pipe == 0
			&& (var->cmds[i]->inbuilt == EXPORT || var->cmds[i]->inbuilt == UNSET
			|| var->cmds[i]->inbuilt == CD))
			break ;
		else if (var->cmds[i]->write_to_pipe == 1 
			&& (var->cmds[i]->inbuilt == CD || var->cmds[i]->inbuilt == EXPORT
			|| var->cmds[i]->inbuilt == UNSET || var->cmds[i]->inbuilt == EXIT))
			i++;
		if (debug_mode < -2)
			print_cmd(var, i);
		set_cmd_path(var->cmds[i], var);
//		if (debug_mode < -3)
//			printf("Execute_cmds.c/370: HERE we are\t COMMAND PATH SET TO: '%s'\n", var->cmds[i]->cmd[0]);
		var->env = create_env_from_list(var->env_list);
		pid = fork();
		if (pid == 0)
			execute_cmd(var->cmds[i], var, i);
		else
		{
			if (var->pipes > 0)
				close_pipes_in_parent(var, i);
			wait_function(pid);
		}
		i++;
	}
}