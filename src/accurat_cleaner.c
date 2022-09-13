/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accurat_cleaner.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 14:29:40 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/13 08:45:46 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	string_free(char **str)
{
	int	j;

	j = 0;
	while (str[j] != NULL)
	{
		free (str[j]);
		str[j] = NULL;
		j++;
	}
	free (str);
	str = NULL;
}

static void	free_var(t_var *var)
{
	if (var->cmds != NULL)
	{
		free (var->cmds);
		var->cmds = NULL;
	}
	if (var->input_escape != NULL)
	{
		free (var->input_escape);
		var->input_escape = NULL;
	}
	if (var->t_input != NULL)
	{
		free (var->t_input);
		var->t_input = NULL;
	}
	if (var->t_escape != NULL)
	{
		free (var->t_escape);
		var->t_escape = NULL;
	}
}

static void	set_to_null(t_cmd *cmd)
{
	cmd->filename_to_read = NULL;
	cmd->filename_to_write = NULL;
	cmd->limiter = NULL;
}

/**
 * @brief Function to clean the t_var structure
 * 
 * @param var 
 */
void	accurat_var_cleaner(t_var *var)
{
	int	i;

	i = 0;
	if (var->cmds != NULL)
	{
		while (var->cmds[i] != NULL)
		{
			if (var->cmds[i]->cmd != NULL)
				string_free(var->cmds[i]->cmd);
			if (var->cmds[i]->cmd_esc != NULL)
				string_free(var->cmds[i]->cmd_esc);
			if (var->cmds[i]->filename_to_read != NULL)
				free (var->cmds[i]->filename_to_read);
			if (var->cmds[i]->filename_to_write != NULL)
				free (var->cmds[i]->filename_to_write);
			if (var->cmds[i]->limiter != NULL)
				free (var->cmds[i]->limiter);
			set_to_null(var->cmds[i]);
			free (var->cmds[i]);
			var->cmds[i] = NULL;
			i++;
		}
	}
	free_var(var);
}

/**
 * @brief Function to free the env variables.
 * 
 * @param var
 */
void	accurat_env_cleaner(t_var *var)
{
	t_list	*temp_next;
	int		i;

	i = 0;
	if (var->env_list != NULL)
	{
		while (var->env_list != NULL)
		{
			free (var->env_list->content);
			var->env_list->content = NULL;
			temp_next = var->env_list->next;
			free (var->env_list);
			var->env_list = NULL;
			var->env_list = temp_next;
			i++;
		}
	}
	if (var->prompt != NULL)
	{
		free (var->prompt);
		var->prompt = NULL;
	}
}
