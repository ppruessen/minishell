/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 13:13:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/02 16:45:47 by pprussen         ###   ########.fr       */
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
static int	mark_variables_to_expand(t_var *var)
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
static char	*expand_special_dollar(t_var *var)
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
//		printf("NUR_DOLLAR mit Wert |%s|\n", var->dollar_value);
		return (var->dollar_value);
	}
	return (NULL);
}

/* Gets a $-variable and writes the value into cmd */
static char	*expand_env(t_var *var)
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

/* stores escape characters in the length of the expanded variable 
in var->dollar_esc_value */
static void	find_esc_value(t_var *var)
{
	int	strlen;
	int	i;

	strlen = ft_strlen(var->dollar_value);
	if (strlen == 0)
	{
		var->dollar_esc_value = "";
		return ;
	}
	if (var->dollar_esc[0] == '$' && var->dollar_esc[1] == '\0')
	{
		var->dollar_esc_value = ft_strdup("-");
		return ;
	}
	i = 1;
	while (i <= strlen)
	{
		if (var->dollar_esc[1] == '5')
			var->dollar_esc_value = ft_strjoin_char(var->dollar_esc_value, '3');
		else if (var->dollar_esc[1] == '4')
			var->dollar_esc_value = ft_strjoin_char(var->dollar_esc_value, '2');
		i++;
	}
}

/* Function extracts valid $VARIABLE and stores it in var->dollar_var
// and stores escape characters in var->dollar_esc accordingly */
static void	extract_dollar_var(t_var *var)
{
	int	i;

	i = 0;
	if (var->dollar_var != NULL)
	{
		free(var->dollar_var);
		var->dollar_var = NULL;
	}
	while (var->t_escape[i])
	{
		if (var->t_escape[i] == '$')
		{
			var->dollar_var = ft_strjoin_char(var->dollar_var,
					var->t_input[i]);
			var->dollar_esc = ft_strjoin_char(var->dollar_esc,
					var->t_escape[i]);
			i++;
			while (var->t_escape[i] == '4' || var->t_escape[i] == '5')
			{
				var->dollar_var = ft_strjoin_char(var->dollar_var,
						var->t_input[i]);
				var->dollar_esc = ft_strjoin_char(var->dollar_esc,
						var->t_escape[i]);
				i++;
			}
			break ;
		}
		i++;
	}
}

/* Functions expands valid $VARIABLES and sets escaped string accordingly */
void	expand_variables(t_var *var)
{
	int	nb_variables;
	int	i;

	nb_variables = mark_variables_to_expand(var);
	i = 0;
	while (i < nb_variables)
	{
		var->dollar_var = NULL;
		var->dollar_value = NULL;
		var->dollar_esc = NULL;
		var->dollar_esc_value = NULL;
		extract_dollar_var(var);
//		printf("Input string alt:\t|%s|\n", var->input);
//		printf("Zahlenkolonne alt:\t|%s|\n", var->dollar_esc);
		if (expand_special_dollar(var) == NULL)
			var->dollar_value = expand_env(var);
//		printf("Var Dollar var :\t|%s|\n", var->dollar_var);
//		printf("soll ersetzt werden durch:\t|%s|\n", var->dollar_value);
		var->t_input = replace_str(var->t_input, var->dollar_var,
				var->dollar_value);
		find_esc_value(var);
//		printf("Var Dollar escape:\t|%s|\n", var->dollar_esc);
//		printf("soll ersetzt werden durch:\t|%s|\n", var->dollar_esc_value);
		var->t_escape = replace_str(var->t_escape, var->dollar_esc,
				var->dollar_esc_value);
//		printf("Input string neu:\t|%s|\n", var->t_input);
//		printf("Zahlenkolonne neu:\t|%s|\n", var->dollar_esc_value);
		i++;
	}
//	printf("Expand variables fertig!\n");
}
