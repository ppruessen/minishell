/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 15:12:23 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/02 16:48:57 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	clean_env(t_var *var)
{
	t_list	*temp;

	if (debug_mode < -3)
		printf("exit.c/20: Clean ENV\n");
	while (var->env_list != NULL)
	{
		temp = var->env_list->next;
		free (var->env_list->content);
		var->env_list->content = NULL;
		free (var->env_list);
		var->env_list = NULL;
		var->env_list = temp;
	}
}

void	clean_up(t_var *var)
{
	int	i;
	int	j;

	clean_env(var);
	i = 0;
	while (var->cmds[i] != NULL)
	{
		if (var->cmds[i]->read_from_file == FROM_HERE_DOC)
			unlink(".bonus_tmp");
		if (var->cmds[i]->write_to_file > 0)
			unlink(var->cmds[i]->filename_to_write);
		if (var->cmds[i]->read_from_file == FROM_FILE)
			unlink(var->cmds[i]->filename_to_read);
		j = 0;
		while (j < var->cmds[i]->nb_cmds)
		{
			free(var->cmds[i]->cmd[j]);
			var->cmds[i]->cmd[j] = NULL;
			j++;
		}
		j = 0;
		while (j < var->cmds[i]->nb_cmds)
		{
			free(var->cmds[i]->cmd_esc[j]);
			var->cmds[i]->cmd_esc[j] = NULL;
			j++;
		}
		free(var->cmds[i]->cmd);
		var->cmds[i]->cmd = NULL;
		free(var->cmds[i]->cmd_esc);
		var->cmds[i]->cmd_esc = NULL;
		free(var->cmds[i]);
		var->cmds[i] = NULL;
		i++;
	}
	free(var->cmds);
	var->cmds = NULL;
	free(var->prompt);
	var->prompt = NULL;
	free(var->input_escape);
	var->input_escape = NULL;
	return ;
}

void	ms_exit(t_var *var, char **cmd)
{
	int			i;
	long long	result;

	result = 0;
	i = 0;
	if (cmd[1] != NULL)
	{
		while (cmd[1][i] != '\0')
		{
			if (cmd[1][i] == '-' || cmd[1][i] == '+')
				i++;
			if (ft_isdigit(cmd[1][i]) == 0)
			{
//				printf("exit\n");
				printf("bash: exit: %s: numeric argument required\n", cmd[1]);
				g_status = 255;
				clean_up(var);
				exit(g_status);
			}
			i++;
		}
		if (cmd[2] != NULL)
		{
			write(2, "exit\nbash: exit: too many arguments\n", 39);
			g_status = 1;
		}
		else
		{
			if (ft_atoll(cmd[1], &result) == 0)
				g_status = result % 256;
//			write(1, "exit\n", 5);
		}
	}
	if (cmd[1] == NULL)
	{
		g_status = 0;
//		write(1, "exit\n", 5);
	}
//	clean_up(var);
	if (debug_mode < 0)
		accurat_cleaner(var);
	if (debug_mode < -1)
	 	check_leaks();
	exit(g_status);
}
