/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accurat_cleaner.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 14:29:40 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/02 18:43:38 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	accurat_var_cleaner(t_var *var)
{
	int	i[2];

	i[0] = 0;
	if (var->cmds != NULL)
	{
		while (var->cmds[i[0]] != NULL)
		{
			//	VAR->CMDS->CMD
			if (var->cmds[i[0]]->cmd != NULL)
			{
				i[1] = 0;
				while (var->cmds[i[0]]->cmd[i[1]] != NULL)
				{
					free (var->cmds[i[0]]->cmd[i[1]]);
					var->cmds[i[0]]->cmd[i[1]] = NULL;
					i[1]++;
				}
			}
			// VAR->CMDS->CMD_ESC
			if (var->cmds[i[0]]->cmd_esc != NULL)
			{
				i[1] = 0;
				while (var->cmds[i[0]]->cmd_esc[i[1]] != NULL)
				{
					free (var->cmds[i[0]]->cmd_esc[i[1]]);
					var->cmds[i[0]]->cmd_esc[i[1]] = NULL;
					i[1]++;
				}
			}
			// VAR->CMDS->FILENAME_TO_READ
			if (var->cmds[i[0]]->filename_to_read != NULL)
			{
				free (var->cmds[i[0]]->filename_to_read);
				var->cmds[i[0]]->filename_to_read = NULL;
			}
			// VAR->CMDS->FILENAME_TO_WRITE
			if (var->cmds[i[0]]->filename_to_write != NULL)
			{
				free (var->cmds[i[0]]->filename_to_write);
				var->cmds[i[0]]->filename_to_write = NULL;
			}
			// VAR->CMDS->LIMITER
			if (var->cmds[i[0]]->limiter != NULL)
			{
				free (var->cmds[i[0]]->limiter);
				var->cmds[i[0]]->limiter = NULL;
			}
			i[0]++;
			free (var->cmds[i[0]]);
			var->cmds[i[0]] = NULL;
		}
	}
	// VAR->CMDS
	if (var->cmds != NULL)
	{
		free (var->cmds);
		var->cmds = NULL;
	}
	// VAR->INPUT does not need to be freed
	if (var->input != NULL)
	{
		var->input = NULL;
	}
	// Var->INPUT_ESCAPE
	if (var->input_escape != NULL)
	{
		free (var->input_escape);
		var->input_escape = NULL;
	}
}

void	accurat_env_cleaner(t_var *var)
{
	// VAR->ENV_LIST
	t_list	*temp_next;

	if (var->env_list != NULL)
	{
		while(var->env_list != NULL)
		{
			free (var->env_list->content);
			var->env_list->content = NULL;
			temp_next = var->env_list->next;
			free (var->env_list);
			var->env_list = NULL;
			var->env_list = temp_next;
		}
	}
	if (var->prompt != NULL)
	{
		free (var->prompt);
		var->prompt = NULL;
	}
	// VAR->ENV
	int	i = 0;
	if (var->env != NULL)
	{
		while (var->env[i] != NULL)
		{
			free (var->env[i]);
			var->env[i] = NULL;
			i++;
		}
		free (var->env);
		var->env = NULL;
	}
	free (var->cmds);
}

void	accurat_finder(t_var *var)
{
	int	i[2];

	printf("-------------------- Accurat_Var_Finder Function -----------------------\n");
	i[0] = 0;
	if (var->cmds != NULL)
	{
		while (var->cmds[i[0]] != NULL)
		{
			//	VAR->CMDS->CMD
			if (var->cmds[i[0]]->cmd != NULL)
			{
				i[1] = 0;
				while (var->cmds[i[0]]->cmd[i[1]] != NULL)
				{
					printf("var.cmds[%d].cmd[%d] = %s\n", i[0], i[1], var->cmds[i[0]]->cmd[i[1]]);
					i[1]++;
				}
			}
			// VAR->CMDS->CMD_ESC
			if (var->cmds[i[0]]->cmd_esc != NULL)
			{
				i[1] = 0;
				while (var->cmds[i[0]]->cmd_esc[i[1]] != NULL)
				{
					printf("var.cmds[%d].cmd_esc[%d] = %s\n", i[0], i[1], var->cmds[i[0]]->cmd_esc[i[1]]);
					i[1]++;
				}
			}
			// VAR->CMDS->FILENAME_TO_READ
			if (var->cmds[i[0]]->filename_to_read != NULL)
				printf("filename to read: %s\n", var->cmds[i[0]]->filename_to_read);
			// VAR->CMDS->FILENAME_TO_WRITE
			if (var->cmds[i[0]]->filename_to_write != NULL)
				printf("filename to write: %s\n", var->cmds[i[0]]->filename_to_write);
			// VAR->CMDS->LIMITER
			if (var->cmds[i[0]]->limiter != NULL)
				printf("limiter: %s\n", var->cmds[i[0]]->limiter);
			i[0]++;
		}
	}
	// VAR->ENV
	i[0] = 0;
	if (var->env != NULL)
	{
		while (var->env[i[0]] != NULL)
		{
			printf("var->env[%d] = %s\n", i[0], var->env[i[0]]);
			i[0]++;
		}
	}
	// VAR->ENV_LIST
	t_list	*temp;
	temp = var->env_list;
	while(var->env_list != NULL)
	{
		printf("var->env_list->content: %s\n", (char *)var->env_list->content);
		var->env_list = var->env_list->next;
	}
	var->env_list = temp;
	// VAR->DIR_LIST
	i[0] = 0;
	if (var->dir_list != NULL)
	{
		while (var->dir_list[i[0]] != NULL)
		{
			printf("var->dir_list[%d] = %s\n", i[0], var->dir_list[i[0]]);
			i[0]++;
		}
	}
	// VAR->PATH
	if (var->path != NULL)
		printf("var->path: %s\n", var->path);
	// VAR->PROMPT
	if (var->prompt != NULL)
		printf("var->prompt: %s\n", var->prompt);
	// VAR->INPUT
	if (var->input != NULL)
		printf("var->input: %s\n", var->input);
	// VAR->INPUT_ESCAPE
	if (var->input_escape != NULL)
		printf("var->input_escape: %s\n", var->input_escape);
	// VAR->T_INPUT
	if (var->t_input != NULL)
		printf("var->t_input: %s\n", var->t_input);
	// VAR->T_ESCAPE
	if (var->t_escape != NULL)
		printf("var->t_escape: %s\n", var->t_escape);
	// VAR->CMDS
	if (var->cmds != NULL)
		printf("var->cmds: NICHT NULL\n");
	// VAR->FD[2]
	if (var->fd != NULL)
	{
		i[0] = 0;
		while (i[0] < var->pipes)
		{
			printf("fd nicht null\n");
			i[0]++;
		}
	}
	// VAR->DOLLAR_VAR
	if (var->dollar_var != NULL)
		printf("var->dollar_var: %s\n", var->dollar_var);
	// VAR->DOLLAR_ESC
	if (var->dollar_esc != NULL)
		printf("var->dollar_esc: %s\n", var->dollar_esc);
	// VAR->DOLLAR_VALUE
	if (var->dollar_value != NULL)
		printf("var->dollar_value: %s\n", var->dollar_value);
	// VAR->DOLLAR_ESC_VALUE;
	if (var->dollar_esc_value != NULL)
		printf("var->dollar_esc_value: %s\n", var->dollar_esc_value);
}