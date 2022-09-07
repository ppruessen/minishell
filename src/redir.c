/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 16:44:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/06 21:45:31 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function that sets the redirections */
void	open_redirections(t_cmd *cmd)
{
	if (cmd->read_from_file != 0)
		open_read_from_file(cmd);
	if (cmd->write_to_file != 0)
		open_write_to_file(cmd);
}

/* Function finds redirections and sets them accordingly */
/* Including the filename or limiter from heredoc*/
int	set_redirections(char *temp_input, char *temp_escaped, t_cmd *cmd,
	t_var *var)
{
	int	i;
	int	start;

	i = 0;
	cmd->filename_to_read = NULL;
	while (temp_input[i] != '\0')
	{
		if (temp_input[i] == '<' && temp_escaped[i] == '3')
		{
			cmd->read_from_file = FROM_FILE;
			temp_escaped[i] = 'F';
			start = i + 1;
			if (temp_input[i + 1] == '<' && temp_escaped[i + 1] == '3')
			{
				cmd->read_from_file = FROM_HERE_DOC;
				temp_escaped[i + 1] = 'F';
				start++;
				cmd->limiter = extract_next_word(temp_input,
						temp_escaped, start);
			}
			else
			{
				cmd->filename_to_read = extract_next_word(temp_input,
						temp_escaped, start);
				cmd->fd_to_read = open(cmd->filename_to_read, O_RDONLY);
				if (cmd->fd_to_read <= 0)
				{
					printf("bash: %s: ", cmd->filename_to_read);
					print_file_error(var, FILE_NOT_FOUND);
					close(cmd->fd_to_read);
					var->cmd_check = FALSE;
					return (-1);
				}
				close(cmd->fd_to_read);
			}
		}
		else if (temp_input[i] == '>' && temp_escaped[i] == '3')
		{
			temp_escaped[i] = 'F';
			start = i + 1;
			if (temp_input[i + 1] == '>' && temp_escaped[i + 1] == '3')
			{
				cmd->write_to_file = INTO_FILE_APPEND;
				temp_escaped[i + 1] = 'F';
				start++;
			}
			else
				cmd->write_to_file = INTO_FILE;
			cmd->filename_to_write = extract_next_word(temp_input,
					temp_escaped, start);
			if (cmd->write_to_file == INTO_FILE)
			{
				unlink(cmd->filename_to_write);
				cmd->fd_to_write = open(cmd->filename_to_write, O_WRONLY
						| O_CREAT, 0777);
			}
			else if (cmd->write_to_file == INTO_FILE_APPEND)
				cmd->fd_to_write = open(cmd->filename_to_write, O_RDWR
						| O_CREAT | O_APPEND, 0644);
			close(cmd->fd_to_write);
		}
	i++;
	}
	return (0);
}
