/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd_structures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 10:46:25 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/30 17:44:02 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function findes redirections and set them accordingly */
void	set_redirections(char *temp_input, char *temp_escaped, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (temp_input[i] != '\0')
	{
		if (temp_input[i] == '<' && temp_escaped[i] == '3')
		{
			if (temp_input[i + 1] == '<' && temp_escaped[i + 1] == '3')
			{
				cmd->read_from_file = FROM_HERE_DOC;
				i +=1;
			}
			else
				cmd->read_from_file = FROM_FILE;
		}
		else if (temp_input[i] == '>' && temp_escaped[i] == '3')
		{
			if (temp_input[i + 1] == '>' && temp_escaped[i + 1] == '3')
			{
				cmd->write_to_file = INTO_FILE_APPEND;
				i +=1;
			}
			else
				cmd->write_to_file = INTO_FILE;
		}
		i++;
	}
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
// and expands variables and copy them in the correct struct. */
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
	set_redirections(var->t_input, var->t_escape, cmd);
	set_pipe_status(var, cmd, cmd_nb);
//	printf("cmd->read_from: %d, cmd->write_to: %d\n", cmd->read_from_file, cmd->write_to_file);
//	printf("cmd->read_from_pipe: %d, cmd->write_to_pipe: %d\n", cmd->read_from_pipe, cmd->write_to_pipe);
	expand_variables(var);
//	printf("temp_input=\t\t'%s'\ntemp_escaped=\t'%s'\n", var->t_input, var->t_escape);
	put_temp_input_to_cmd(var, cmd);
//	printf("temp_input=\t\t'%s'\ntemp_escaped=\t'%s'\n", var->t_input, var->t_escape);
	free(var->t_input);
	free(var->t_escape);
}

/* Initialise cmd struct */
void	init_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->read_from_file = 0;
	cmd->write_to_file = 0;
	cmd->read_from_pipe = 0;
	cmd->write_to_pipe = 0;
	cmd->fd_to_read = 0;
	cmd->fd_to_write = 0;
}

/*
// Function to allocate enough memory for the command structures and 
// put in the commands seperated by unescaped whitespaces
*/
void	fill_cmd_structures(t_var *var)
{
	int	i;

	var->index = 0;
//	var->cmds = (t_cmd **)malloc(sizeof(t_cmd *) * (var->pipes + 1));
	var->cmds= (t_cmd **)ft_calloc((var->pipes + 2), sizeof(t_cmd *));
	i = 0;
	while (i < var->pipes + 1)
	{
//		var->cmds[i] = (t_cmd *)malloc(sizeof(t_cmd) * (var->pipes + 1));
		var->cmds[i] = (t_cmd *)ft_calloc((var->pipes + 2), sizeof(t_cmd));
		init_cmd(var->cmds[i]);
		put_str_to_struct(var, var->cmds[i], i);
		i++;
	}

}