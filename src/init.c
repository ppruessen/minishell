/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 16:53:25 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/06 16:59:17 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Initialise cmd struct */
void	init_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->nb_cmds = 0;
	cmd->inbuilt = NOT_SET;
	cmd->read_from_file = 0;
	cmd->write_to_file = 0;
	cmd->read_from_pipe = 0;
	cmd->write_to_pipe = 0;
	cmd->fd_to_read = 0;
	cmd->fd_to_write = 0;
	cmd->filename_to_read = NULL;
	cmd->filename_to_write = NULL;
	cmd->limiter = NULL;
}
