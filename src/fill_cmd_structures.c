/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd_structures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 10:46:25 by mschiman          #+#    #+#             */
/*   Updated: 2022/08/20 17:39:266 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Returns the next word within a string cutting off whitespaces and sets */
/* the escape string to zero on the equivalent spaces of that word. */
static char	*extract_next_word(char *full_str, char *esc_str, int start)
{
	int		stop;
	int		quotes;
	char	*res_str;

	res_str = NULL;
	if (debug_mode < -3)
		printf("fill_cmd_structures.c/27 FULL STRING: \t|%s|\n", full_str);
	if (debug_mode < -3)
		printf("fill_cmd_structures.c/29 ESC STRING: \t|%s|\n", esc_str);
	quotes = 0;
	while (ft_iswhitespace(full_str[start]) == 1)
		start++;
	if (esc_str[start] == '0')
	{
		while (esc_str[start] == '0')
			start++;
		quotes = 1;
	}
	stop = start;
	if (quotes == 0)
	{
		while (full_str[stop] && ft_iswhitespace(full_str[stop]) == 0)
			stop++;
	}
	else
	{
		while (esc_str[stop] != '0')
			stop++;
	}
	if (start != stop)
		res_str = ft_substr(full_str, (unsigned int) start, (stop - start));
	else
		res_str = NULL;
	while (start < stop)
	{
		esc_str[start] = 'W';
		start++;
	}
	return (res_str);
}

/* Function findes redirections and set them accordingly */
static int	set_redirections(char *temp_input, char *temp_escaped, t_cmd *cmd,
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
				cmd->limiter = extract_next_word(temp_input, temp_escaped, start);
				if (debug_mode < -3)
					printf("fill_cmd_structures.c/85 limiter: |%s|\n", cmd->limiter);
			}
			else
			{
				cmd->filename_to_read = extract_next_word(temp_input, temp_escaped, start);
				if (debug_mode < -3)
					printf("fill_cmd_structures.c/91 Filename to open: |%s|\n", cmd->filename_to_read);
				cmd->fd_to_read = open(cmd->filename_to_read, O_RDONLY);
				if (debug_mode < -3)
					printf("fill_cmd_structures.c/94 FD to read: %i\n", cmd->fd_to_read);
				if (cmd->fd_to_read <= 0)
				{
					if (debug_mode < -3)
					{
						printf("fill_cmd_structures.c/98 FD to read: %i\n", cmd->fd_to_read);
						printf("fill_cmd_structures.c/99 Filename to open: |%s|\n", cmd->filename_to_read);
					}
					printf("bash: %s: ", cmd->filename_to_read);
					print_file_error(var, FILE_NOT_FOUND);
					close(cmd->fd_to_read);
//					g_status = 1;
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
			cmd->filename_to_write = extract_next_word(temp_input, temp_escaped, start);
			if (cmd->write_to_file == INTO_FILE)
			{
				unlink(cmd->filename_to_write);
				cmd->fd_to_write = open(cmd->filename_to_write, O_WRONLY | O_CREAT, 0777);
			}
			else if (cmd->write_to_file == INTO_FILE_APPEND)
				cmd->fd_to_write = open(cmd->filename_to_write, O_RDWR | O_CREAT | O_APPEND, 0644);
			close(cmd->fd_to_write);
		}
	i++;
	}
	if (debug_mode < -3)
	{
		printf("fill_cmd_structures.c/138 G-Status: |%i|\n", g_status);
		printf("fill_cmd_structures.c/139 Temp input:\t |%s|\t\n", temp_input);
		printf("fill_cmd_structures.c/140 Temp escape:\t |%s|\n", temp_escaped);
	}
	return (0);
}

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

/* Function splits string at pipes into substrings,
// sets redirection flags accordingly
// and expands variables and copies them in the correct struct. */
void	put_str_to_struct(t_var *var, t_cmd *cmd, int cmd_nb)
{
	int		i;

	if (var->index > 0)
		var->index++;
	i = var->index;
	while (var->input[i] != '\0')
	{
		if (var->input[i] == '|' && (var->input_escape[i] == '3'))
			break;
		i++;
	}
	var->t_input = (char *)malloc(sizeof(char) * (i - var->index) + 1);
	ft_strlcpy(var->t_input, &var->input[var->index], i - var->index + 1);
	var->t_escape = (char *)malloc(sizeof(char) * (i - var->index) + 1);
	ft_strlcpy(var->t_escape, &var->input_escape[var->index], i - var->index + 1);
	var->index = i;
	if (set_redirections(var->t_input, var->t_escape, cmd, var) == 0)
	{
		set_pipe_status(var, cmd, cmd_nb);
		expand_variables(var);
		put_temp_input_to_cmd(var, cmd);
	}
	free(var->t_input);
	var->t_input = NULL;
	free(var->t_escape);
	var->t_escape = NULL;
}

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

/*
// Function to allocate enough memory for the command structures and 
// put in the commands seperated by unescaped whitespaces
*/
void	fill_cmd_structures(t_var *var)
{
	int	i;

	var->index = 0;
	var->cmds = (t_cmd **)ft_calloc((var->pipes + 2), sizeof(t_cmd *));
	i = 0;
	while (i < var->pipes + 1)
	{
		var->cmds[i] = NULL;
		var->cmds[i] = (t_cmd *)ft_calloc((var->pipes + 2), sizeof(t_cmd));
		init_cmd(var->cmds[i]);
		put_str_to_struct(var, var->cmds[i], i);
		i++;
	}
 }
