/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 16:30:11 by mschiman          #+#    #+#             */
/*   Updated: 2022/08/30 21:02:59 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Catches ||	;	&	(	) */
static int	syntax_error_check_not_available_1(t_var *var)
{
	int	i;

	i = 0;
	while (var->input[i] != '\0')
	{
		if (var->input[i] == '|' && var->input[i + 1] == '|'
		&& var->input_escape[i] == '3' && var->input_escape[i + 1] == '3')
		{
			print_error(var, DOUBLE_PIPES);
			return (1);
		}
		else if (var->input[i] == ';' && var->input_escape[i] == '3')
		{
			print_error(var, SEMICOLON);
			return (1);
		}
		else if (var->input[i] == '&' && var->input_escape[i] == '3')
		{
			print_error(var, AMPERSAND);
			return (1);
		}
		else if ((var->input[i] == '(' || var->input[i] == ')')
			&& var->input_escape[i] == '3')
		{
			print_error(var, BRACES);
			return (1);
		}
		i++;
	}
	return (0);
}

/* Catches	*	and	!	*/
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
//	g_status = -42;
	var->cmd_check = TRUE;
	return (0);
}

/* Catches more than two << or >> */
static int	syntax_error_check_redir(t_var *var)
{
	int	i;

	i = 0;
	while (var->input[i] != '\0')
	{
		if (var->input[i] == '>' && var->input_escape[i] == '3')
		{
			if (var->input[i + 1] == '>' && var->input_escape[i + 1] == '3')
			{
				if (var->input[i] == '>' && var->input_escape[i] == '3')
				{
					print_error(var, TOO_MANY_REDIR_OUT_TWO);
					return (1);
				}
				i += 2;
			}
			else
				i += 1;
		}
		if (ft_iswhitespace(var->input[i]) == 1 && var->input_escape[i] == '3')
			i = whitespace_runner(var->input, var->input_escape, i) + 1;
		if (var->input[i] == '<' && var->input_escape[i] == '3')
		{
			if (var->input[i + 1] == '<' && var->input_escape[i + 1] == '3')
				i += 2;
			else
				i += 1;
		}
		if (ft_iswhitespace(var->input[i]) == 1 && var->input_escape[i] == '3')
			i = whitespace_runner(var->input, var->input_escape, i) + 1;
		if (var->input[i] == '<' && var->input_escape[i] == '3')
		{
			if (var->input[i + 1] == '<' && var->input_escape[i + 1] == '3')
				print_error(var, TOO_MANY_REDIR_IN_TWO);
			else
				g_status = 1;
			return (1);
		}
		i++;
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
