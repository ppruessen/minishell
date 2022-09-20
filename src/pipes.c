/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 15:54:31 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/20 13:32:28 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Functions detects if input or/and output have to be piped */
void	set_pipe_status(t_var *var, t_cmd *cmd, int cmd_nb)
{
	cmd->read_from_pipe = FALSE;
	cmd->write_to_pipe = FALSE;
	if (var->pipes != 0)
	{
		if (cmd_nb == 0)
			cmd->write_to_pipe = TRUE;
		else if (var->pipes == cmd_nb)
			cmd->read_from_pipe = TRUE;
		else
		{
			cmd->read_from_pipe = TRUE;
			cmd->write_to_pipe = TRUE;
		}
	}
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
	pid_t	pid;

	pid = getpid();
	if (cmd->write_to_pipe == TRUE && cmd->read_from_pipe == TRUE)
	{
		printf("Mittlerer Teil: pid = %d\n", pid);
		if (dup2(var->fd[i - 1][READ], STDIN_FILENO) == -1)
		{
			printf("Unable to dup2 var->fd[%i][READ]\n", i - 1);
			return ;
		}
		//if (var->fd[i - 1][READ] != STDIN_FILENO)
		//	close(var->fd[i - 1][READ]);
		if (dup2(var->fd[i][WRITE], STDOUT_FILENO) == -1)
		{
			printf("Unable to dup2 var->fd[%i][WRITE]\n", i);
			return ;
		}
		//if (var->fd[i][WRITE] != STDOUT_FILENO)
		//	close (var->fd[i][WRITE]);
		close_all_read_pipes(var);
		
	}
	else if (cmd->write_to_pipe == TRUE)
	{
		pid = getpid();
		printf("Startteil: pid %d\n", pid);
		if (dup2(var->fd[i][WRITE], STDOUT_FILENO) == -1)
		{
			printf("Unable to dup2 var->fd[%i][WRITE]\n", i);
			return ;
		}
	//	if (var->fd[i][WRITE] != STDOUT_FILENO)
			close (var->fd[i][WRITE]);
		
		close_all_read_pipes(var);
	}
	else if (cmd->read_from_pipe == TRUE)
	{
		pid = getpid();
		printf("Endteil : pid: %d\n", pid);
		if (dup2(var->fd[i - 1][READ], STDIN_FILENO) == -1)
		{
			printf("Unable to dup2 var->fd[%i][READ]\n", i - 1);
			return ;
		}
		//if (var->fd[i - 1][READ] != STDIN_FILENO)
		//	close(var->fd[i - 1][READ]);
//		close_all_read_pipes(var);
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
