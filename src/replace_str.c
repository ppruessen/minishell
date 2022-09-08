/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 13:04:03 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 10:40:04 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function to init the rpl struct */
static void	init_rpl(t_replace *rpl, char *full_str,
	char *old_part, char *new_part)
{
	rpl->i = 0;
	rpl->j = 0;
	rpl->start = 0;
	rpl->end = 0;
	rpl->strlen_full = ft_strlen(full_str);
	rpl->strlen_old_part = ft_strlen(old_part);
	rpl->strlen_new_part = ft_strlen(new_part);
}

static void	replace_string_2(t_replace *rpl, char *full_str, char *new_part)
{
	rpl->j = 0;
	rpl->i = 0;
	while (rpl->i < rpl->start)
	{
		rpl->new_str[rpl->j] = full_str[rpl->i];
		rpl->i++;
		rpl->j++;
	}
	rpl->i = 0;
	while (rpl->i < rpl->strlen_new_part)
	{
		rpl->new_str[rpl->j] = new_part[rpl->i];
		rpl->i++;
		rpl->j++;
	}
	rpl->i = rpl->end;
	while (rpl->i < rpl->strlen_full)
	{
		rpl->new_str[rpl->j] = full_str[rpl->i];
		rpl->i++;
		rpl->j++;
	}
}

void	malloc_new_str(t_replace *rpl)
{
	rpl->strlen_new_str = rpl->strlen_full
		- (rpl->end - rpl->start) + rpl->strlen_new_part;
	rpl->new_str = (char *) ft_calloc((rpl->strlen_new_str + 1), sizeof (char));
	if (rpl->new_str == NULL)
		exit (0);
}

int	set_vars(t_replace *rpl, char *full_str, char *old_part)
{
	rpl->start = rpl->i;
	while (full_str[rpl->i] == old_part[rpl->j] && rpl->j
		< rpl->strlen_old_part && full_str[rpl->i] != '\0')
	{
		rpl->i++;
		rpl->j++;
	}
	if (rpl->j == (rpl->strlen_old_part))
	{
		rpl->end = rpl->i;
		return (0);
	}
	else
		rpl->j = 0;
	if (full_str[rpl->i] != '\0')
		rpl->i++;
	return (1);
}

char	*replace_str(char *full_str, char *old_part, char *new_part)
{
	t_replace	rpl;

	if (full_str == NULL || old_part == NULL)
		return (NULL);
	if (full_str[0] == '\0')
		return ("");
	init_rpl(&rpl, full_str, old_part, new_part);
	while (full_str[rpl.i] != '\0')
	{
		if (set_vars(&rpl, full_str, old_part) == 0)
			break ;
	}
	if (rpl.j == 0)
		return (full_str);
	malloc_new_str(&rpl);
	replace_string_2(&rpl, full_str, new_part);
	free(full_str);
	return (rpl.new_str);
}
