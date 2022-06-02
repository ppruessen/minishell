/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 13:04:03 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/27 17:29:06 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	init_vars(int i, int j, int start, int end)
{
	i = 0;
	j = 0;
	start = 0;
	end = 0;
}

static char **check_where_to_insert(int strlen_old_part, int strlen_new_part,
	int strlen_full, char *full_str)
{
	int	i;
	int	j;
	int	start;
	int	end;

	init_vars(i, j, start, end);
	strlen_full = ft_strlen(full_str);
	strlen_old_part = ft_strlen(old_part);
	strlen_new_part = ft_strlen(new_part);
	while (full_str[i] != '\0')
	{
		start = i;
		while (full_str[i] == old_part[j] && j < strlen_old_part && full_str[i] != '\0')
		{
			i++;
			j++;
		}
		if (j == (strlen_old_part))
		{
			end = i;
			break ;
		}
		else
			j = 0;
		if (full_str[i] != '\0')
			i++;
	}
	if (j == 0)
		return (-1);
	else
		return (0);
}

char	*replace_str(char *full_str, char *old_part, char *new_part)
{
	char	*new_str;
	int		strlen_full;
	int		strlen_old_part;
	int		strlen_new_part;
	int		strlen_new_str;
	
	if (full_str == NULL || old_part == NULL || new_part == NULL)
		return (NULL);
	if (full_str[0] == '\0')
		return ("");
	if (check_where_to_insert(strlen_old_part, strlen_new_part, strlen_full, full_str) == -1)
		return (full_str);
	strlen_new_str = strlen_full - (end - start) + strlen_new_part;
	new_str = (char *) ft_calloc((strlen_new_str + 1), sizeof (char));
	if (new_str == NULL)
		exit (0); //exit & error
	j = 0;
	i = 0;
	while (i < start) 
	{
		new_str[j] = full_str[i];
		i++;
		j++;
	}
	i = 0;
	while (i < strlen_new_part)
	{
		new_str[j] = new_part[i];
		i++;
		j++;
	}
	i = end;
	while (i < strlen_full)
	{
		new_str[j] = full_str[i];
		j++;
		i++;
	}
	free(full_str);
	return (new_str);
}
