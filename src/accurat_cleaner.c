/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accurat_cleaner.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 14:29:40 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/05 11:54:08 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/**
 * @brief Function to clean the t_var structure
 * 
 * @param var 
 */
void	accurat_var_cleaner(t_var *var)
{
	int	i;
	int	j;

	i = 0;
	if (var->cmds != NULL)
	{
		while (var->cmds[i] != NULL)
		{
			//	VAR->CMDS->CMD
			if (var->cmds[i]->cmd != NULL)
			{
				j = 0;
				while (var->cmds[i]->cmd[j] != NULL)
				{
					free (var->cmds[i]->cmd[j]);
					var->cmds[i]->cmd[j] = NULL;
					j++;
				}
				free (var->cmds[i]->cmd);
				var->cmds[i]->cmd = NULL;
			}
			// VAR->CMDS->CMD_ESC
			if (var->cmds[i]->cmd_esc != NULL)
			{
				j = 0;
				while (var->cmds[i]->cmd_esc[j] != NULL)
				{
					free (var->cmds[i]->cmd_esc[j]);
					var->cmds[i]->cmd_esc[j] = NULL;
					j++;
				}
				free (var->cmds[i]->cmd_esc);
				var->cmds[i]->cmd_esc = NULL;
			}
			// VAR->CMDS->FILENAME_TO_READ
			if (var->cmds[i]->filename_to_read != NULL)
			{
				free (var->cmds[i]->filename_to_read);
				var->cmds[i]->filename_to_read = NULL;
			}
			// VAR->CMDS->FILENAME_TO_WRITE
			if (var->cmds[i]->filename_to_write != NULL)
			{
				free (var->cmds[i]->filename_to_write);
				var->cmds[i]->filename_to_write = NULL;
			}
			// VAR->CMDS->LIMITER
			if (var->cmds[i]->limiter != NULL)
			{
				free (var->cmds[i]->limiter);
				var->cmds[i]->limiter = NULL;
			}
			free (var->cmds[i]);
			var->cmds[i] = NULL;
			i++;
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
	// VAR->T_INPUT
	if (var->t_input != NULL)
	{
		free (var->t_input);
		var->t_input = NULL;
	}
	// VAR_>T_ESCAPE
		if (var->t_escape != NULL)
	{
		free (var->t_escape);
		var->t_escape = NULL;
	}
}
/**
 * @brief Function to free the env variables.
 * 
 * @param var
 */
void	accurat_env_cleaner(t_var *var)
{
	// VAR->ENV_LIST
	t_list	*temp_next;
	int	i = 0;

	if (var->env_list != NULL)
	{
		while(var->env_list != NULL)
		{
//			printf("var->env_list->content = %s\n", (char *)var->env_list->content);
			free (var->env_list->content);
			var->env_list->content = NULL;
			temp_next = var->env_list->next;
//			if (var->env_list->next == NULL)
//				printf("var-env-list-next = NULL: i = %d\n", i);
			free (var->env_list);
			var->env_list = NULL;
			var->env_list = temp_next;
			i++;
		}
//		printf("i = %d\n", i);
	}
	if (var->prompt != NULL)
	{
		free (var->prompt);
		var->prompt = NULL;
	}
	// VAR->ENV
	// i = 0;
	// if (var->env != NULL)
	// {
	// 	if (var->env[i] != NULL)
	// 	{
	// 		while (var->env[i] != NULL)
	// 		{
	// 			printf("ACCURAT ENV CLEANER i = %d\n", i);
	// 			free (var->env[i]);
	// 			var->env[i] = NULL;
	// 			i++;
	// 			if (var->env[i] == NULL)
	// 				printf("var->env == NULL\n");
	// 		}
	// 	}
	// 	printf("ENDE ACCURAT ENV CLEANER\n");
	// 	free (var->env);
	// 	var->env = NULL;
	//}
}

/**
 * @brief Function to find variables that are not NULL
 * 
 * @param var 
 */
void	accurat_finder(t_var *var)
{
	int	i;
	int	j;

	printf("-------------------- Accurat_Var_Finder Function -----------------------\n");
	i = 0;
	if (var->cmds != NULL)
	{
		while (var->cmds[i] != NULL)
		{
			//	VAR->CMDS->CMD
			if (var->cmds[i]->cmd != NULL)
			{
				j = 0;
				while (var->cmds[i]->cmd[j] != NULL)
				{
					printf("var.cmds[%d].cmd[%d] = %s\n", i, j, var->cmds[i]->cmd[j]);
					j++;
				}
			}
			// VAR->CMDS->CMD_ESC
			if (var->cmds[i]->cmd_esc != NULL)
			{
				j = 0;
				while (var->cmds[i]->cmd_esc[j] != NULL)
				{
					printf("var.cmds[%d].cmd_esc[%d] = %s\n", i, j, var->cmds[i]->cmd_esc[j]);
					j++;
				}
			}
			// VAR->CMDS->FILENAME_TO_READ
			if (var->cmds[i]->filename_to_read != NULL)
				printf("filename to read: %s\n", var->cmds[i]->filename_to_read);
			// VAR->CMDS->FILENAME_TO_WRITE
			if (var->cmds[i]->filename_to_write != NULL)
				printf("filename to write: %s\n", var->cmds[i]->filename_to_write);
			// VAR->CMDS->LIMITER
			if (var->cmds[i]->limiter != NULL)
				printf("limiter: %s\n", var->cmds[i]->limiter);
			i++;
		}
	}
	// VAR->ENV
	i = 0;
	if (var->env != NULL)
	{
		while (var->env[i] != NULL)
		{
			printf("var->env[%d] = %s\n", i, var->env[i]);
			i++;
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
	i = 0;
	if (var->dir_list != NULL)
	{
		while (var->dir_list[i] != NULL)
		{
			printf("var->dir_list[%d] = %s\n", i, var->dir_list[i]);
			i++;
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
		i = 0;
		while (i < var->pipes)
		{
			printf("fd nicht null\n");
			i++;
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