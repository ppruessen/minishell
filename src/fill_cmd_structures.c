/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd_structures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 10:46:25 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 13:50:55 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

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
			break ;
		i++;
	}
	var->t_input = (char *)malloc(sizeof(char) * (i - var->index) + 1);
	ft_strlcpy(var->t_input, &var->input[var->index], i - var->index + 1);
	var->t_escape = (char *)malloc(sizeof(char) * (i - var->index) + 1);
	ft_strlcpy(var->t_escape, &var->input_escape[var->index],
		i - var->index + 1);
	var->index = i;
	if (set_redirections(var->t_input, var->t_escape, cmd, var) == 0)
	{
		set_pipe_status(var, cmd, cmd_nb);
		expand_variables(var);
		put_temp_input_to_cmd(var, cmd);
	}
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
