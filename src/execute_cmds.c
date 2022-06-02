/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 14:43:18 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/30 18:08:28 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	print_cmd(t_var *var, int i)
{
	int	j;

	printf("----------------------COMMAND[%i]------------------------\n", i);
	printf("var->cmds[%d]->inbuilt = %d\n", i, var->cmds[i]->inbuilt);
	printf("var->cmds[%d]->read_from_file = %d\n", i, var->cmds[i]->read_from_file);
	printf("var->cmds[%d]->write_to_file = %d\n", i, var->cmds[i]->write_to_file);
	printf("var->cmds[%d]->read_from_pipe = %d\n", i, var->cmds[i]->read_from_pipe);
	printf("var->cmds[%d]->write_to_pipe = %d\n", i, var->cmds[i]->write_to_pipe);
	printf("var->pipes = %d\n", var->pipes);
	j = 0;
	while(var->cmds[i]->cmd[j] != NULL)
	{
		printf("CMD[%d]: %s\n", j, var->cmds[i]->cmd[j]);
		j++;
	}
	printf("--------------------------------------------------------\n");
}

/* Function finds inbuilts and sets cmd->inbuilts to:
// exit = 1, echo = 2, env = 3, pwd = 4, export = 5, unset = 6, cd = 7 */
void	find_inbuilt(t_cmd *cmd, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 'e' && str[i + 1] == 'x' && str[i + 2] == 'i' &&
			str[i + 3] == 't' && str[i + 4] == '\0')
			cmd->inbuilt = EXIT;
		else if (str[i] == 'e' && str[i + 1] == 'c' && str[i + 2] == 'h' &&
			str[i + 3] == 'o' && str[i + 4] == '\0')
			cmd->inbuilt = ECHO;
		else if (str[i] == 'e' && str[i + 1] == 'n' && str[i + 2] == 'v' &&
			str[i + 3] == '\0')
			cmd->inbuilt = ENV;
		else if (str[i] == 'p' && str[i + 1] == 'w' && str[i + 2] == 'd' &&
			str[i + 3] == '\0')
			cmd->inbuilt = PWD;
		else if (str[i] == 'e' && str[i + 1] == 'x' && str[i + 2] == 'p' &&
			str[i + 3] == 'o' && str[i + 4] == 'r' && str[i + 5] == 't' &&
			str[i + 6] == '\0')
			cmd->inbuilt = EXPORT;
		else if (str[i] == 'u' && str[i + 1] == 'n' && str[i + 2] == 's' &&
			str[i + 3] == 'e' && str[i + 4] == 't' && str[i + 5] == '\0')
			cmd->inbuilt = UNSET;
		else if (str[i] == 'c' && str[i + 1] == 'd' && str[i + 2] == '\0')
			cmd->inbuilt = CD;
		i++;
	}
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
	free(cmd_path);
	pwd_temp = ft_strjoin_char(pwd_temp, '/');
	cmd_path = ft_strjoin(pwd_temp, cmd_path_temp);
	free(pwd_temp);
	free(cmd_path_temp);
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
	free(var->path);
	while (var->dir_list[i] != NULL)
	{
		temp = ft_strjoin(var->dir_list[i], "/");
		free(var->dir_list[i]);
		var->dir_list[i] = temp;
		i++;
	}
}

/* Checks if the command is found within the path directories */
char	*check_and_set_path(char *cmd, t_var *var)
{
	char	*valid_cmd_path;
	int		i;

	get_path(var);
	gen_dir_list(var);
	i = 0;
	while (var->dir_list[i] != NULL)
	{
		valid_cmd_path = ft_strjoin(var->dir_list[i], cmd);
		if (access(valid_cmd_path, X_OK) == 0)
		{
			free(var->dir_list);
			free(cmd);
			return (valid_cmd_path);
		}
		i++;
	}
	return (NULL);
}

/* Sets the cmd path to unbuilt functions */
char	*set_inbuilt_path(t_cmd *cmd, t_var *var)
{
	if (cmd->inbuilt == 2 || cmd->inbuilt == 3 || cmd->inbuilt == 4)
		free(cmd->cmd[0]);
	if (cmd->inbuilt == 2)
		return(ft_strjoin(var->pwd, "/incl/inbuilts_progs/echo"));
	else if (cmd->inbuilt == 3)
		return(ft_strjoin(var->pwd, "/incl/inbuilts_progs/env"));
	else if (cmd->inbuilt == 4)
		return(ft_strjoin(var->pwd, "/incl/inbuilts_progs/pwd"));
	return (cmd->cmd[0]);
}

/* Sets the cmd path */
void	set_cmd_path(t_cmd *cmd, t_var *var)
{
	if (cmd->inbuilt == 0)
	{
		if (cmd->cmd[0][0] == '.')
			cmd->cmd[0] = create_rel_path(cmd->cmd[0]);
		else if (cmd->cmd[0][0] == '/')
			return ;
		else
			cmd->cmd[0] = check_and_set_path(cmd->cmd[0], var);
	}
	else
		cmd->cmd[0] = set_inbuilt_path(cmd, var);

}

/* Function that opens the file descriptor to the read file */
void	open_read_from_file(t_cmd *cmd)
{
	if (cmd->read_from_file == 1)
	{
		if (cmd->write_to_file != 0)
			cmd->fd_to_read = open(cmd->cmd[cmd->nb_cmds - 2], O_RDONLY);
		else
			cmd->fd_to_read = open(cmd->cmd[cmd->nb_cmds - 1], O_RDONLY);
		dup2(cmd->fd_to_read, STDIN_FILENO);
		close(cmd->fd_to_read);
	}
	if (cmd->read_from_file == 2)
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
	if (cmd->write_to_file == 1)
	{
		unlink(cmd->cmd[cmd->nb_cmds -1]);
		cmd->fd_to_write = open(cmd->cmd[cmd->nb_cmds -1], O_WRONLY | O_CREAT, 0777);
		dup2(cmd->fd_to_write, STDOUT_FILENO);
		close(cmd->fd_to_write);
	}
	else if (cmd->write_to_file == 2)
	{
		cmd->fd_to_write = open(cmd->cmd[cmd->nb_cmds -1], O_RDWR | O_CREAT | O_APPEND, 0644);
		dup2(cmd->fd_to_write, STDOUT_FILENO);
		close(cmd->fd_to_write);
	}
}

/* Function that sets the pipes and filedescriptors */
void	open_redirections(t_cmd *cmd)
{
	if (cmd->read_from_file != 0)
	{
		open_read_from_file(cmd);
		if (cmd->read_from_file == 2)
			cmd->cmd[cmd->nb_cmds - 1] = NULL;
	}
	if (cmd->write_to_file != 0)
		open_write_to_file(cmd);
	if (cmd->write_to_file != 0 && cmd->read_from_file != 0)
	{
		cmd->cmd[cmd->nb_cmds - 2] = NULL;
		cmd->cmd[cmd->nb_cmds - 1] = NULL;
	}
	if (cmd->write_to_file != 0)
			cmd->cmd[cmd->nb_cmds - 1] = NULL;
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
		if (var->pipes > 0)
			set_pipes_in_child(var, cmd, i);
		open_redirections(cmd);
		if (execve(cmd->cmd[0], &cmd->cmd[0], var->env) == -1)
		{
			printf("FEHLER IM EXECVE\n");
			exit(0);
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
		printf("In execute_cmds.c/338: exit status was %d\n", es);
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

	printf("HERE we are 343\n");
	if (var->pipes > 0)
		open_filedescriptors(var);
	printf("HERE we are 346\n");
	i = 0;
	if (var->cmds == NULL)
		return ;
	while (var->cmds[i] != NULL)
	{
		find_inbuilt(var->cmds[i], var->cmds[i]->cmd[0]);
		printf("HERE we are 353\n");
		print_cmd(var, i);
		printf("HERE we are 355\n");
		set_cmd_path(var->cmds[i], var);
		printf("HERE we are 357\n");
		printf("COMMAND PATH: '%s'\n", var->cmds[i]->cmd[0]);
		var->env = create_env_from_list(var->env_list);
		printf("HERE we are 360\n");
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