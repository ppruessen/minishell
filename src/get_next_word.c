/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 13:50:19 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/08 14:41:53 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static int	whitespace_jumper(char *full_str, int stop)
{
	while (full_str[stop] && ft_iswhitespace(full_str[stop]) == 0)
		stop++;
	return (stop);
}

static int	null_jumper(char *esc_str, int stop)
{
	while (esc_str[stop] != '0')
		stop++;
	return (stop);
}

static int	null_finder(char *esc_str, int start)
{
	while (esc_str[start] == '0')
		start++;
	return (start);
}

/* Returns the next word within a string cutting off whitespaces and sets */
/* the escape string to zero on the equivalent spaces of that word. */
char	*get_next_word(char *full_str, char *esc_str, int start)
{
	int		stop;
	int		quotes;
	char	*res_str;

	res_str = NULL;
	quotes = 0;
	while (ft_iswhitespace(full_str[start]) == 1)
		start++;
	if (esc_str[start] == '0')
	{	
		start = null_finder(esc_str, start);
		quotes = 1;
	}
	stop = start;
	if (quotes == 0)
		stop = whitespace_jumper(full_str, stop);
	else
		stop = null_jumper(esc_str, stop);
	if (start != stop)
		res_str = ft_substr(full_str, (unsigned int) start, (stop - start));
	else
		res_str = NULL;
	while (start < stop)
		esc_str[start++] = 'W';
	return (res_str);
}
