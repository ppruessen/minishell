/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 16:30:11 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/07 11:27:25 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/**
 * @brief Catches ||	;	&	(	)
 * 
 * @param var 
 * @return int 
 */
static int	syntax_error_check_not_available_1(t_var *var)
{
	int	i;

	i = 0;
	while (var->input[i] != '\0')
	{
		if (var->input[i] == '|' && var->input[i + 1] == '|'
			&& var->input_escape[i] == '3' && var->input_escape[i + 1] == '3')
			return (print_error(var, DOUBLE_PIPES));
		else if (var->input[i] == ';' && var->input_escape[i] == '3')
			return (print_error(var, SEMICOLON));
		else if (var->input[i] == '&' && var->input_escape[i] == '3')
			return (print_error(var, AMPERSAND));
		else if ((var->input[i] == '(' || var->input[i] == ')')
			&& var->input_escape[i] == '3')
			return (print_error(var, BRACES));
		i++;
	}
	return (0);
}

/**
 * @brief Catches	*	and	!
 * 
 * @param var [t_var *]
 * @return int 1 if * or ! is found, else 0
 */
static int	syntax_error_check_not_available_2(t_var *var)
{
	int	i;

	i = 0;
	while (var->input[i] != '\0')
	{
		if (var->input[i] == '*' && var->input_escape[i] == '3')
		{
			print_error(var, ASTERISK);
			return (1);
		}
		if (var->input[i] == '!' && var->input_escape[i] == '3')
		{
			print_error(var, EXCLAMATION);
			return (1);
		}
		i++;
	}
	var->cmd_check = TRUE;
	return (0);
}

/**
 * @brief Catches <	>	and single | with nothing after them
 * 
 * @param var [t_var *]
 * @return int 
 */
static int	syntax_error_check_newline(t_var *var)
{
	int	i;
	int	found;
	int	whitespace;

	i = 0;
	found = 0;
	whitespace = 0;
	while (var->input[i])
	{
		if ((var->input[i] == '|' || var->input[i] == '<'
				|| var->input[i] == '>') && var->input_escape[i] == '3')
		{
			found = 1;
			whitespace = 1;
		}
		if (var->input[i] == '|' && var->input_escape[i] == '3')
			found++;
		else if (ft_iswhitespace(var->input[i]) == 0)
			whitespace = 0;
		i++;
	}
	if (found > 0 && whitespace == 1)
		return (found);
	return (0);
}

/**
 * @brief Checks for syntax errors in the cmd line input und prints
 * error messages.
 * 
 * @param var 
 */
void	syntax_error_check(t_var *var)
{
	if (syntax_error_check_not_available_1(var) == 1)
	{
		var->cmd_check = FALSE;
		return ;
	}
	if (syntax_error_check_not_available_2(var) == 1)
	{
		var->cmd_check = FALSE;
		return ;
	}
	if (syntax_error_check_redir(var) == 1)
	{
		var->cmd_check = FALSE;
		return ;
	}
	if (syntax_error_check_newline(var) == 1)
	{
		var->cmd_check = FALSE;
		print_error(var, UNEXPECTED_TOKEN_NL);
	}
	else if (syntax_error_check_newline(var) > 1)
	{
		var->cmd_check = FALSE;
		print_error(var, UNEXPECTED_TOKEN_PIPE);
	}
}
