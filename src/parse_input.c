/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:21:03 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/30 15:36:32 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function sets var->squotes and var->dquotes respectively
// and set var->input_escape[i] to '0' */
static void	set_quote_vars(t_var *var, int quote, int i)
{
	if (quote == S_QUOTES && var->squotes == CLOSED)
		var->squotes = OPEN;
	else if (quote == S_QUOTES && var->squotes == OPEN)
		var->squotes = CLOSED;
	else if (quote == D_QUOTES && var->dquotes == CLOSED)
		var->dquotes = OPEN;
	else if (quote == D_QUOTES && var->dquotes == OPEN)
		var->dquotes = CLOSED;
	var->input_escape[i] = '0';
}

/* Function to check quotes and create var->input_escape with:
// 0: will be deleted
// 1: char in single quotes
// 2: char in double quotes
// 3: char is not escaped
*/
static int	check_quotes(t_var *var)
{
	int	i;
	var->squotes = FALSE;
	var->dquotes = FALSE;

	var->input_escape = (char *)malloc(sizeof(char) * 
		(ft_strlen(var->input) + 1));
	i = 0;
	while (var->input[i])
	{
		if (var->input[i] == '\'' && var->dquotes == CLOSED)
			set_quote_vars(var, S_QUOTES, i);
		else if (var->input[i] == '"' && var->squotes == CLOSED)
			set_quote_vars(var, D_QUOTES, i);
		else if (var->squotes == OPEN)
			var->input_escape[i] = '1';
		else if (var->dquotes == OPEN)
			var->input_escape[i] = '2';
		else
			var->input_escape[i] = '3';
		i++;
	}
	var->input_escape[i] = '\0';
	if (var->squotes == OPEN || var->dquotes == OPEN)
		return (OPEN);
	return (CLOSED);
}

/* Function counts unescaped pipes and stores it var->pipes */
static void	count_pipes(t_var *var)
{
	int	i;

	var->pipes = 0;
	i = 0;
	while (var->input[i])
	{
		if (var->input[i] == '|' && var->input_escape[i] == '3')
			var->pipes++;
		i++;
	}
}

int	parse_input(t_var *var)
{
	int	open_quotes;

	open_quotes = check_quotes(var);
	if (open_quotes == OPEN)
	{
		print_error(OPEN_QUOTES);
		return (0);
	}
	syntax_error_check(var);
	if (g_status == -42)
	{
		count_pipes(var);
		fill_cmd_structures(var);
	}
	return (0);
}