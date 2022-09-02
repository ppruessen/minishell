/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 14:13:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/02 19:13:58 by pprussen         ###   ########.fr       */
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