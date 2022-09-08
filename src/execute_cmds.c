/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 14:43:18 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 14:15:15 by pprussen         ###   ########.fr       */
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

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failed");
		return ;
	}
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
	if (var->pipes > 0)
		set_pipes_in_child(var, cmd, i);
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
	if (execve(*execve_input, execve_input, var->env) == -1)
	{
		print_cmd_error(var, CMD_NOT_FOUND, execve_input[0]);
		exit(127);
	}
}

/* Function that prepares the execution of the command in a fork */
/* Inbuilts don't go into the execve*/
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
			wait_function(pid);
		}
		i++;
	}
}
