/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 15:51:27 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/06 22:48:04 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function thas generates a directory list from path*/
void	gen_dir_list(t_var *var)
{
	int		i;
	char	*temp;

	i = 0;
	var->dir_list = ft_split(var->path, ':');
	free (var->path);
	var->path = NULL;
	while (var->dir_list[i] != NULL)
	{
		temp = ft_strjoin(var->dir_list[i], "/");
		free (var->dir_list[i]);
		var->dir_list[i] = NULL;
		var->dir_list[i] = temp;
		i++;
	}
}

/* Frees the directory list and the cmd */
void	free_dir_list_cmd(char **dir_list, char *cmd)
{
	int	i;

	i = 0;
	while (dir_list[i] != NULL)
	{
		free (dir_list[i]);
		dir_list[i] = NULL;
		i++;
	}
	free (dir_list);
	dir_list = NULL;
	free (cmd);
	cmd = NULL;
}
