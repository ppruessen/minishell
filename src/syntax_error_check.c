/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 16:30:11 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/27 13:38:08 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Catches ||	;	&	(	) */
static int	syntax_error_check_not_available_1(t_var *var, int i)
{
	if (var->input[i] == '|' && var->input[i + 1] == '|'
		&& var->input_escape[i] == '3' && var->input_escape[i + 1] == '3')
	{
		print_error(DOUBLE_PIPES);
		return (1);
	}
	else if (var->input[i] == ';' && var->input_escape[i] == '3')
	{
		print_error(SEMICOLON);
		return (1);
	}
	else if (var->input[i] == '&' && var->input_escape[i] == '3')
	{
		print_error(AMPERSAND);
		return (1);
	}
	else if ((var->input[i] == '(' || var->input[i] == ')')
		&& var->input_escape[i] == '3')
	{
		print_error(BRACES);
		return (1);
	}
	return (0);
}

/* Catches	*	and	!	*/
static int	syntax_error_check_not_available_2(t_var *var, int i)
{
	if (var->input[i] == '*' && var->input_escape[i] == '3')
	{
		print_error(ASTERISK);
		return (1);
	}
	if (var->input[i] == '!' && var->input_escape[i] == '3')
	{
		print_error(EXCLAMATION);
		return (1);
	}
	g_status = -42;
	return (0);
}

/* Catches more than two << or >> */
static int	syntax_error_check_redir(t_var *var, int i)
{
	if (var->input[i] == '>' && var->input[i + 1] == '>'
		&& var->input[i + 2] == '>' && var->input_escape[i] == '3'
		&& var->input_escape[i + 1] == '3' && var->input_escape[i + 2] == '3')
	{
		print_error(TOO_MANY_REDIR_OUT);
		return (1);
	}
	else if (var->input[i] == '<' && var->input[i + 1] == '<'
		&& var->input[i + 2] == '<' && var->input_escape[i] == '3'
		&& var->input_escape[i + 1] == '3'
		&& var->input_escape[i + 2] == '3')
	{
		print_error(TOO_MANY_REDIR_IN);
		return (1);
	}
	return (0);
}

/* Catches <	>	and single | with nothing after them */
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

/* Checks for syntax errors in the cmd line input und prints error messages. */
void	syntax_error_check(t_var *var)
{
	int	i;

	i = 0;
	while (var->input[i])
	{
		if (syntax_error_check_not_available_1(var, i) == 1)
			return ;
		if (syntax_error_check_not_available_2(var, i) == 1)
			return ;
		if (syntax_error_check_redir(var, i) == 1)
			return ;
		i++;
	}
	if (syntax_error_check_newline(var) == 1)
		print_error(UNEXPECTED_TOKEN_NL);
	else if (syntax_error_check_newline(var) > 1)
		print_error(UNEXPECTED_TOKEN_PIPE);
}
