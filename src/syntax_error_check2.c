/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 10:54:32 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/08 12:18:55 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/**
 * @brief Calculates i 
 * 
 * @param var 
 * @param i 
 * @return int 
 */
static int	i_calculator(t_var *var, int i)
{
	if (var->input[i + 1] == '<' && var->input_escape[i + 1] == '3')
		i += 2;
	else
		i += 1;
	return (i);
}

/**
 * @brief 
 * 
 * @param var 
 * @param i 
 * @return int 
 */
static int	to_many_redir_check1(t_var *var, int i)
{
	if (var->input[i + 1] == '<' && var->input_escape[i + 1] == '3')
		print_error(var, TOO_MANY_REDIR_IN_TWO);
	else
		g_status = 1;
	return (1);
}

/**
 * @brief 
 * 
 * @param var 
 * @param i 
 * @return int 
 */
static int	to_many_redir_check2(t_var *var, int i)
{
	if (var->input[i + 1] == '>' && var->input_escape[i + 1] == '3')
	{
		if (var->input[i + 2] == '>' && var->input_escape[i] == '3')
			return (-1);
		return (i += 2);
	}
	else
		return (i += 1);
}

static int	whitespace_runner2(char *input, char *escaped, int i)
{
	if (ft_iswhitespace(input[i]) != 1)
		return (i);
	while (ft_iswhitespace(input[i]) == 1 && escaped[i] == '3')
		i++;
	return (i - 1);
}

/**
 * @brief Catches more than two << or >>
 * 
 * @param var [t_var *]
 * @return int 
 */
int	syntax_error_check_redir(t_var *var)
{
	int	i;

	i = 0;
	while (var->input[i] != '\0')
	{
		if (var->input[i] == '>' && var->input_escape[i] == '3')
		{
			i = to_many_redir_check2(var, i);
			if (i == -1)
				return (print_error(var, TOO_MANY_REDIR_OUT_TWO));
		}
		if (ft_iswhitespace(var->input[i]) == 1 && var->input_escape[i] == '3')
			i = whitespace_runner2(var->input, var->input_escape, i) + 1;
		if (var->input[i] == '<' && var->input_escape[i] == '3')
			i = i_calculator(var, i);
		if (ft_iswhitespace(var->input[i]) == 1 && var->input_escape[i] == '3')
			i = whitespace_runner2(var->input, var->input_escape, i) + 1;
		if (var->input[i] == '<' && var->input_escape[i] == '3')
			return (to_many_redir_check1(var, i));
		i++;
	}
	return (0);
}
