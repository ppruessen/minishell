/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_inbuilts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:19:56 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/06 11:21:23 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

static void	find_inbuilt_exit(t_var *var, t_cmd *cmd, char *str, int i)
{
	if (str[i] == 'e' && str[i + 1] == 'x' && str[i + 2] == 'i'
		&& str[i + 3] == 't' && str[i + 4] == '\0')
	{
		cmd->inbuilt = EXIT;
		if (cmd->write_to_pipe != 1)
			ms_exit(var, cmd->cmd);
	}
}

static void	find_inbuilt_progs(t_cmd *cmd, char *str, int i)
{
	if (str[i] == 'e' && str[i + 1] == 'c' && str[i + 2] == 'h'
		&& str[i + 3] == 'o' && str[i + 4] == '\0')
		cmd->inbuilt = ECHO;
	else if (str[i] == 'e' && str[i + 1] == 'n' && str[i + 2] == 'v'
		&& str[i + 3] == '\0')
		cmd->inbuilt = ENV;
	else if (str[i] == 'p' && str[i + 1] == 'w' && str[i + 2] == 'd'
		&& str[i + 3] == '\0')
		cmd->inbuilt = PWD;
}

/* Function finds inbuilts and sets cmd->inbuilts to:
// exit = 1, echo = 2, env = 3, pwd = 4, export = 5, unset = 6, cd = 7 */
void	find_inbuilts(t_var *var, t_cmd *cmd, char *str)
{
	int	i;

	i = 0;
	if (!str || var->cmd_check == FALSE)
		return ;
	find_inbuilt_exit(var, cmd, str, i);
	if (str[i] == 'e' && str[i + 1] == 'x' && str[i + 2] == 'p'
		&& str[i + 3] == 'o' && str[i + 4] == 'r' && str[i + 5] == 't'
		&& str[i + 6] == '\0')
	{
		cmd->inbuilt = EXPORT;
		if (debug_mode < -3)
			printf("find_inbuilds.c/54 Export Function erkannt\n");
		if (cmd->write_to_pipe != 1)
			export_var(var, cmd->cmd, cmd->cmd_esc);
	}	
	else if (str[i] == 'u' && str[i + 1] == 'n' && str[i + 2] == 's'
		&& str[i + 3] == 'e' && str[i + 4] == 't' && str[i + 5] == '\0')
	{
		cmd->inbuilt = UNSET;
		if (cmd->write_to_pipe != 1)
			unset_env(var, cmd->cmd);
	}
	else if (str[i] == 'c' && str[i + 1] == 'd' && str[i + 2] == '\0')
	{
		cmd->inbuilt = CD;
		if (cmd->write_to_pipe != 1)
			change_directory(cmd->cmd);
	}
	find_inbuilt_progs(cmd, str, i);
	if (debug_mode < -3)
		printf("find_inbulits.c/70: str = %s\n", str);
}
