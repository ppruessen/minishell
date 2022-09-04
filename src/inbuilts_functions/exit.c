/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 15:12:23 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/04 19:51:59 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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
//				clean_up(var);
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
	accurat_var_cleaner(var);
	accurat_env_cleaner(var);
	if (debug_mode < 0)
		accurat_finder(var);
	if (debug_mode < -1)
	 	check_leaks();
	exit(g_status);
}
