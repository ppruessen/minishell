/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 15:48:06 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/26 12:42:29 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

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
		cmd->fd_to_write = open(cmd->filename_to_write, O_WRONLY
				| O_CREAT, 0777);
		dup2(cmd->fd_to_write, STDOUT_FILENO);
		close(cmd->fd_to_write);
	}
	else if (cmd->write_to_file == INTO_FILE_APPEND)
	{
		cmd->fd_to_write = open(cmd->filename_to_write, O_RDWR
				| O_CREAT | O_APPEND, 0644);
		dup2(cmd->fd_to_write, STDOUT_FILENO);
		close(cmd->fd_to_write);
	}
}
