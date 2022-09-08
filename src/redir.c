/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 16:44:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 14:42:40 by pprussen         ###   ########.fr       */
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

static int	set_read_from_file(t_cmd *cmd, t_var *var, t_redir *re)
{
	cmd->read_from_file = FROM_FILE;
	re->escape[re->i] = 'F';
	re->start = re->i + 1;
	if (re->input[re->i + 1] == '<' && re->escape[re->i + 1] == '3')
	{
		cmd->read_from_file = FROM_HERE_DOC;
		re->escape[re->i + 1] = 'F';
		re->start++;
		cmd->limiter = get_next_word(re->input, re->escape, re->start);
	}
	else
	{
		cmd->filename_to_read = get_next_word(re->input, re->escape, re->start);
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
	return (0);
}

static void	set_write_to_file(t_cmd *cmd, t_redir *re)
{
	re->escape[re->i] = 'F';
	re->start = re->i + 1;
	if (re->input[re->i + 1] == '>' && re->escape[re->i + 1] == '3')
	{
		cmd->write_to_file = INTO_FILE_APPEND;
		re->escape[re->i + 1] = 'F';
		re->start++;
	}
	else
		cmd->write_to_file = INTO_FILE;
	cmd->filename_to_write = get_next_word(re->input,
			re->escape, re->start);
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

/* Function finds redirections and sets them accordingly */
/* Including the filename or limiter from heredoc*/
int	set_redirections(char *temp_input, char *temp_escaped, t_cmd *cmd,
	t_var *var)
{
	t_redir	re;

	re.i = 0;
	re.input = temp_input;
	re.escape = temp_escaped;
	cmd->filename_to_read = NULL;
	while (re.input[re.i] != '\0')
	{
		if (re.input[re.i] == '<' && re.escape[re.i] == '3')
		{
			if (set_read_from_file(cmd, var, &re) == -1)
				return (-1);
		}
		else if (re.input[re.i] == '>' && re.escape[re.i] == '3')
			set_write_to_file(cmd, &re);
		re.i++;
	}
	return (0);
}
