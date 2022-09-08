/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_variables_to_expand.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 14:00:46 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/08 14:03:24 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function checks if the c charakter is allowed in a variable name */
int	is_valid_var_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_' || c == '$' || c == '?')
		return (1);
	return (0);
}

/* Marks $Variables in escape string with the c char */
static int	mark_escaped(t_var *var, int i, char c)
{
	while (is_valid_var_char(var->t_input[i]) == 1)
	{
		var->t_escape[i] = c;
		i++;
	}
	return (i - 1);
}

static int	find_dollar(char *input, char *escape, int i)
{
	if (input[i] == '$' && (input[i + 1] == ' '
			|| input[i + 1] == '\t' || input[i + 1] == '\n'
			|| input[i + 1] == '\0')
		&& (escape[i] == '2' || escape[i] == '3')
		&& (escape[i + 1] == '2' || escape[i + 1] == '3'
			|| escape[i + 1] == '\0'))
		return (TRUE);
	return (FALSE);
}

/* Function finds $VARIABLES that have to be expanded and marks them in 
// escaped with the '4' character for $VARIABLES in double quotes and
// the '5' character for $VARIABLES outside of quotes;
// Returns amount of found $VARIABLES */
int	mark_variables_to_expand(t_var *var)
{
	int	i;
	int	found_dollar_var;

	found_dollar_var = 0;
	i = 0;
	while (var->t_input[i] != '\0')
	{
		if (find_dollar(var->t_input, var->t_escape, i) == TRUE)
			i += 2;
		if (var->t_input[i] == '$' && var->t_escape[i] == '2')
		{
			var->t_escape[i] = '$';
			i = mark_escaped(var, i + 1, '4');
			found_dollar_var++;
		}
		else if (var->t_input[i] == '$' && var->t_escape[i] == '3')
		{
			var->t_escape[i] = '$';
			i = mark_escaped(var, i + 1, '5');
			found_dollar_var++;
		}
		i++;
	}
	return (found_dollar_var);
}
