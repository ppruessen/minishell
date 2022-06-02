/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 15:12:23 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/30 16:42:31 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

static void	clean_up(t_var *var)
{
	int	i;

	i = 0;
	while (var->cmds[i] != NULL)
	{
	/*	if (var->cmds[i]->read_from_file == 2)
		{
			unlink(".bonus_tmp");
			free(var->fd_to_read);
		}
		if (var->cmds[i]->write_to_file > 0
			|| var->cmds[i]->read_from_file == 1)
		{
			unlink(var->cmds[i]->cmd[var->cmds->nb_cmds]]);
			unlink(var->cmds[i]->cmd[var->cmds->nb_cmds - 1]]);
			free(var->fd_to_write);
		}
		free(var->cmds);*/
		i++;
	}
	//free(var);
	return ;
}

void	close_everything(t_var *var, char **cmd)
{
	int			i;
	long long	result;

	result = 0;
	i = 0;
	if (cmd[1] != NULL)
	{
		write(2, "bash: exit: too many arguments\n", 31);
		g_status = 1;
	}
	while (cmd[0][i] != '\0')
	{
		if (ft_isdigit(cmd[0][i]) == 0)
		{
			printf("bash: exit: %s: numeric argument required\n", cmd[0]);
			g_status = 255;
			clean_up(var);
			exit(g_status);
		}
		i++;
	}
	if (ft_atoll(cmd[0], &result) == 0)
		g_status = result % 256;
	clean_up(var);
	exit(g_status);
}
