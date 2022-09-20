/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 14:43:18 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/20 15:43:38 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Checks the input for inbuilts, marks them and makes sure that they don't */
/* go into the execve*/
/* Values for inbuilts: EXPORT:5, UNSET:6, CD:7, EXIT:1*/
static int	catch_inbuilts(t_var *var, int i)
{
	find_inbuilts(var, var->cmds[i], var->cmds[i]->cmd[0]);
	if (var->cmds[i]->write_to_pipe == 0 && var->cmds[i]->inbuilt >= 5)
		return (0);
	else if (var->cmds[i]->write_to_pipe == 1 && (var->cmds[i]->inbuilt >= 5
			|| var->cmds[i]->inbuilt == 1))
		return (1);
	return (-1);
}

/* Removes <, > and Filenames from the String to feed it then into execve*/
char	**generate_execve_input(t_cmd *cmd, int input_members)
{
	char	**execve_input;
	int		j;
	int		k;

	execve_input = (char **) malloc(((input_members + 1) * sizeof(char *)) + 1);
	if (execve_input == NULL)
		return (NULL);
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
	return (execve_input);
}

/* Function that waits for the child process */
static void	wait_function(pid_t pid)
{
	int			status;
	int			es;

	(void)pid;
	while ((wait(&status)) > 0)
		;
	if (WIFEXITED(status))
	{
		es = WEXITSTATUS(status);
		if (es == 255)
			g_status = 127;
		g_status = es;
	}
}

/* Executes the cmd */
void	execute_cmd(t_cmd *cmd, t_var *var, int i)
{
	int		j;
	int		input_members;
	char	**execve_input;

	set_cmd_path(var->cmds[i], var);
	var->env = create_env_from_list(var->env_list);
//	if (var->pipes > 0)
//		set_pipes_in_child(var, cmd, i);
	open_redirections(cmd);
	execve_input = NULL;
	input_members = 0;
	j = 0;
	while (cmd->cmd[j] != NULL)
	{
		if (cmd->cmd_esc[j][0] != 'F' && (cmd->cmd_esc[j][0] != 'W'))
			input_members++;
		j++;
	}
	execve_input = generate_execve_input(cmd, input_members);
	//printf("Kommen wir bis hierher?\n");
	if (execve(*execve_input, execve_input, var->env) == -1)
	{
		print_cmd_error(var, CMD_NOT_FOUND, execve_input[0]);
		accurat_child_cleaner(execve_input, var);
		exit(127);
	}
}

/* Function that prepares the execution of the command in a fork */
/* Inbuilts don't go into the execve*/
void	execute_cmds(t_var *var)
{
	int		i;
	pid_t	pid;
//	int		fds[2];
	int saved_stdin;



//	if (var->pipes > 0)
//		open_filedescriptors(var);
	i = 0;
	if (var->cmds == NULL)
		return ;
	saved_stdin = fcntl(STDIN_FILENO, F_DUPFD_CLOEXEC, 0);
	while (var->cmds[i + 1] != NULL)
	{
		int		fds[2];
		if (pipe(fds) < 0)
		{
			perror("pipe(2) error");
			break ;
		}
		if ((pid = fork()) < 0)
		{
			perror("fork(2) error");
			break ;
		}
        /* Parent needs to close the pipe's write channel to make sure
         * we don't hang. Parent reads from the pipe's read channel.
         */
		if (pid > 0)
		{
			if (close(fds[WRITE]) < 0)
			{
				perror("close(2) error");
				break;
			}
			if (dup2(fds[READ], STDIN_FILENO) < 0)
			{
				perror("dupPipe() error");
				break;
			}
		}
		/* Child Process */
		else
		{
//			if (var->cmds[i]->read_from_file == TRUE)
				open_read_from_file(var->cmds[i]);
//			if (var->cmds[i]->write_to_file == TRUE)
				open_write_to_file(var->cmds[i]);
			if (close(fds[READ]) < 0)
			{
				perror("close(2) error");
				break;
			}
			if (dup2(fds[WRITE], STDOUT_FILENO) < 0)
			{
				perror("dupPipe() error");
				break;
			}
		}
		i++;
	}
	while (var->cmds[i] != NULL)
	{
		printf("Zweite While loop 173.\n");
		open_read_from_file(var->cmds[i]);
		open_write_to_file(var->cmds[i]);
		if (catch_inbuilts(var, i) == 0)
			break ;
		else if (catch_inbuilts(var, i) == 1)
			i++;
		pid = fork();
		if (pid == 0)
			execute_cmd(var->cmds[i], var, i);
		// else
		// {
		// 	if (var->pipes > 0)
		// 		close_pipes_in_parent(var, i);
		// }
		i++;
	}
	wait_function(pid);
	if (dup2(saved_stdin, STDIN_FILENO) < 0)
		perror("dup2(2) error when attempting to restore stdin");
	if (close(saved_stdin) < 0)
	{
		perror("close(2) failed on saved_stdin");
	}
}



/*		
	while (var->cmds[i + 1] != NULL)
	{
		if (catch_inbuilts(var, i) == 0)
			break ;
		else if (catch_inbuilts(var, i) == 1)
			i++;
		pid = fork();
		if (pid == 0)
			execute_cmd(var->cmds[i], var, i);
		else
		{
			if (var->pipes > 0)
				close_pipes_in_parent(var, i);
		}
		i++;
	}
	wait_function(pid);
}
*/