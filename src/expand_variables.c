/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 13:13:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 13:24:34 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

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
	free(var->dollar_var);
	var->dollar_var = NULL;
	while (var->t_escape[i])
	{
		if (var->t_escape[i] == '$')
		{
			var->dollar_var = ft_strjoin_char(var->dollar_var, var->t_input[i]);
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
		if (expand_special_dollar(var) == NULL)
			var->dollar_value = expand_env(var);
		var->t_input = replace_str(var->t_input, var->dollar_var,
				var->dollar_value);
		find_esc_value(var);
		var->t_escape = replace_str(var->t_escape, var->dollar_esc,
				var->dollar_esc_value);
		i++;
	}
}
