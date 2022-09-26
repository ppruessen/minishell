/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 15:54:31 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/26 13:12:47 by pprussen         ###   ########.fr       */
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
		{
			cmd->write_to_pipe = TRUE;
		}
		else if (var->pipes == cmd_nb)
		{
			cmd->read_from_pipe = TRUE;
		}
		else
		{
			cmd->read_from_pipe = TRUE;
			cmd->write_to_pipe = TRUE;
		}
	}
}

/* Function that closes all READ pipes in child process */
void	close_all_pipes(t_var *var)
{
	int	j;

	j = 0;
	while (j < var->pipes)
	{
		if (close(var->fd[j][READ]) == -1)
		{
			printf("Unable to close var->fd[%d][READ]\n", j);
		}
		if (close(var->fd[j][WRITE]) == -1)
		{
			printf("Unable to close var->fd[%d][WRITE]\n", j);
		}
		j++;
	}
}

/* Function set the pipes in the child process */
void	set_pipes_in_child(t_var *var, t_cmd *cmd, int i)
{
	if (cmd->write_to_pipe == TRUE && cmd->read_from_pipe == TRUE)
	{
		if (dup2(var->fd[i - 1][READ], STDIN_FILENO) == -1)
			printf("Unable to dup2 var->fd[%i][READ]\n", i - 1);
		if (dup2(var->fd[i][WRITE], STDOUT_FILENO) == -1)
			printf("Unable to dup2 var->fd[%i][WRITE]\n", i);
		close_all_pipes(var);
	}
	else if (cmd->write_to_pipe == TRUE)
	{
		if (dup2(var->fd[i][WRITE], STDOUT_FILENO) == -1)
			printf("Unable to dup2 var->fd[%i][WRITE]\n", i);
		close_all_pipes(var);
	}
	else if (cmd->read_from_pipe == TRUE)
	{
		if (dup2(var->fd[i - 1][READ], STDIN_FILENO) == -1)
			printf("Unable to dup2 var->fd[%i][READ]\n", i - 1);
		close_all_pipes(var);
	}
}

/* Funnction to close all pipes in parent process */
void	close_pipes_in_parent(t_var *var, int end)
{
	int	i;

	i = 0;
	while (i < end)
	{
		if (var->cmds[i]->write_to_pipe == 1)
		{
			close(var->fd[i][WRITE]);
		}
		else
		{
			close(var->fd[i - 1][READ]);
		}
		i++;
	}
	close(var->fd[0][READ]);
}
