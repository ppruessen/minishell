/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 14:13:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/29 11:25:13 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	copy_env(t_var *var, char **env)
{
	int		i;

	var->env_list = NULL;
	i = 0;	
	while (env[i] != NULL)
	{
		ft_lstadd_back(&var->env_list, ft_lstnew(ft_strdup(env[i])));
		i++;
	}
}