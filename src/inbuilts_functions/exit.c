/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 15:12:23 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/07 09:30:23 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief exit for non numeric arguments in exit call
 * 
 * @param var [t_cmd *]
 * @param cmd [char *]
 */
static void	non_numeric_exit(t_var *var, char *cmd)
{
	printf("bash: exit: %s: numeric argument required\n", cmd);
	g_status = 255;
	accurat_var_cleaner(var);
	accurat_env_cleaner(var);
	exit(g_status);
}

/**
 * @brief prints the error to many arguments to std_err
 * 
 */
static void	print_to_many_arguments(void)
{
	write(2, "exit\nbash: exit: too many arguments\n", 39);
	g_status = 1;
}

/**
 * @brief Set the g status object
 * 
 * @param cmd [char *]
 * @param result [long long *]
 */
static void	set_g_status(char *cmd, long long *result)
{
	if (ft_atoll(cmd, result) == 0)
		g_status = *result % 256;
}

/**
 * @brief checks the exit arguments and calls the requiered functions
 * and exits the minishell
 * 
 * @param var [t_var *]
 * @param cmd [char **]
 */
void	ms_exit(t_var *var, char **cmd)
{
	int			i;
	long long	result;

	i = 0;
	g_status = 0;
	if (cmd[1] != NULL)
	{
		while (cmd[1][i] != '\0')
		{
			if (cmd[1][i] == '-' || cmd[1][i] == '+')
				i++;
			if (ft_isdigit(cmd[1][i]) == 0)
				non_numeric_exit(var, cmd[1]);
			i++;
		}
		if (cmd[2] != NULL)
			print_to_many_arguments();
		else
			set_g_status(cmd[1], &result);
	}
	accurat_var_cleaner(var);
	accurat_env_cleaner(var);
	exit(g_status);
}
