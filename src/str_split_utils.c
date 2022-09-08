/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 10:27:34 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/08 10:28:38 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	find_start(t_var *var, int start)
{
	while (var->t_input[start])
	{
		if (var->t_input[start] != ' ' && var->t_input[start] != '\t'
			&& var->t_input[start] != '\n' && var->t_escape[start] == '3')
			return (start);
		else if (var->t_escape[start] == 'F' || var->t_escape[start] == 'W')
			return (start);
		else if (var->t_escape[start] == '0'
			&& (var->t_escape[start + 1] == '2'
				|| var->t_escape[start + 1] == '1'))
			return (start + 1);
		else if (var->t_escape[start] == '0' && var->t_escape[start + 1] == '0')
			return (start);
		start++;
	}
	return (start);
}

int	find_end(t_var *var, int end)
{
	while (var->t_input[end])
	{
		if ((var->t_input[end] == ' ' || var->t_input[end] == '\t'
				|| var->t_input[end] == '\n') && var->t_escape[end] == '3')
			return (end);
		else if (var->t_escape[end] == 'F' && var->t_escape[end + 1] != 'F')
			return (end + 1);
		else if (var->t_escape[end] == 'W' && var->t_escape[end + 1] != 'W')
			return (end + 1);
		else if ((var->t_escape[end] == '2' || var->t_escape[end] == '1')
			&& var->t_escape[end + 1] == '0')
			return (end + 1);
		else if (var->t_escape[end] == '0' && var->t_escape[end + 1] == '0')
		{
			if (var->t_escape[end + 2] == '\0')
				return (end);
			return (end + 1);
		}
		end++;
	}
	return (end);
}
