/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accurat_child_cleaner.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 09:19:44 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/13 10:12:18 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	free_execve_input(char **execve_input)
{
	int	i;

	i = 0;
	while (execve_input[i] != NULL)
	{
		free (execve_input[i]);
		execve_input[i] = NULL;
		i++;
	}
	free (execve_input);
	execve_input = NULL;
}

static void	free_dir_list(t_var *var)
{
	int	i;

	i = 0;
	while (var->dir_list[i] != NULL)
	{
		free (var->dir_list[i]);
		var->dir_list[i] = NULL;
		i++;
	}
	free (var->dir_list);
	var->dir_list = NULL;
}

void	accurat_child_cleaner(char **execve_input, t_var *var)
{
	int	i;

	if (execve_input != NULL)
		free_execve_input(execve_input);
	if (var->dir_list != NULL)
		free_dir_list(var);
	if (var->env != NULL)
	{
		i = 0;
		while (var->env[i] != NULL)
		{
			free (var->env[i]);
			var->env[i] = NULL;
			i++;
		}
		free (var->env);
		var->env = NULL;
	}
	accurat_var_cleaner(var);
	accurat_env_cleaner(var);
}
