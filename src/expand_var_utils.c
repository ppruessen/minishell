/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 21:54:01 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 14:01:14 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

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
