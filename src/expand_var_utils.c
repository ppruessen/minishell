/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 21:54:01 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/06 22:18:48 by mschiman         ###   ########.fr       */
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
		if (var->t_input[i] == '$' && (var->t_input[i + 1] == ' '
				|| var->t_input[i + 1] == '\t' || var->t_input[i + 1] == '\n'
				|| var->t_input[i + 1] == '\0')
			&& (var->t_escape[i] == '2' || var->t_escape[i] == '3')
			&& (var->t_escape[i + 1] == '2' || var->t_escape[i + 1] == '3'
				|| var->t_escape[i + 1] == '\0'))
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

/* Covers edge cases like	$$	$? or only $ */
char	*expand_special_dollar(t_var *var)
{
	if (var->dollar_var[0] == '$' && var->dollar_var[1] == '$')
	{
		var->dollar_value = ft_itoa(getpid());
		return (var->dollar_value);
	}
	else if (var->dollar_var[0] == '$' && var->dollar_var[1] == '?')
	{
		var->dollar_value = ft_itoa(g_status);
		return (var->dollar_value);
	}
	else if (var->dollar_var[0] == '$' && (var->dollar_var[1] == '\t'
			|| var->dollar_var[1] == ' ' || var->dollar_var[1] == '\0'))
	{
		var->dollar_value = ft_strdup("$");
		return (var->dollar_value);
	}
	return (NULL);
}

/* Gets a $-variable and writes the value into cmd */
char	*expand_env(t_var *var)
{
	t_list	*list;
	char	*dollar_var;

	list = var->env_list;
	if (ft_strlen(var->dollar_var) == 1)
		return ("$");
	dollar_var = ft_strtrim(var->dollar_var, "$");
	while (list)
	{
		if (!ft_strncmp(dollar_var, list->content, ft_strlen(dollar_var))
			&& ((char *)(list->content))[ft_strlen(dollar_var)] == '=')
		{
			var->dollar_value = ft_strdup(&((char *)(list->content))
				[ft_strlen(dollar_var) + 1]);
			return (var->dollar_value);
		}
		list = list->next;
	}
	var->dollar_value = ft_strdup("");
	return (var->dollar_value);
}
