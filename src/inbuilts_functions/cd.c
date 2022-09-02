/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 14:22:28 by mschiman          #+#    #+#             */
/*   Updated: 2022/08/30 22:46:17 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	change_directory(char **str)
{
	if (str[1] == NULL)
	{
		rl_on_new_line();
		g_status = 0;
		return ;
	}
	else
	{
		g_status = chdir ((const char *) str[1]);
		if (g_status != 0)
		{
			g_status = 1;
			ft_putstr_fd("bash: cd: ", 1);
			ft_putstr_fd(str[1], 1);
			write(1, ": ", 2);
			perror("");
		}
	}
}
