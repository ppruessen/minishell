/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 21:05:49 by mschiman          #+#    #+#             */
/*   Updated: 2022/04/16 13:08 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

static void	remove_element(t_var *var, t_list *list, int i, t_list *prev)
{
	if (i == 0)
	{
		var->env_list = list->next;
		free(list->content);
		free(list);
	}
	else if (i == ft_lstsize(var->env_list) - 1)
	{
		prev->next = NULL;
		free(list->content);
		free(list);
	}
	else
	{
		prev->next = list->next;
		free(list->content);
		free(list);
	}
}

void	unset_var(t_var *var, char *str)
{
	int		i;
	int		j;
	t_list	*list;
	t_list	*prev;
	char	*temp_str;

	list = var->env_list;
	prev = list;
	i = 0;
	while (i < ft_lstsize(var->env_list))
	{
		j = 0;
		temp_str = (char *)list->content;
		while (temp_str[j] == str[j])
			j++;
		if (temp_str[j] == '=' && str[j] == '\0')
		{
			remove_element(var, list, i, prev);
			break ;
		}
		i++;
		prev = list;
		list = list->next;
	}
}

void	unset_env(t_var *var, char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
	{
		unset_var(var, cmd[i]);
		i++;
	}
	g_status = 0;
}
