/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env_from_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 11:38:41 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/07 14:07:09 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char	**create_env_from_list(t_list *env_list)
{
	int		i;
	t_list	*list;
	int		counter;
	char	**env;

	list = env_list;
	counter = ft_lstsize(env_list);
	env = (char **)malloc(sizeof(char *) * (counter + 1));
	i = 0;
	while (i < counter)
	{
		env[i] = ft_strdup(list->content);
		list = list->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}
