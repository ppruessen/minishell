/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_temp_input_to_cmd_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 12:36:40 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/08 12:40:43 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function that runs over whitespaces and returns the index of the
// last whitespace in a line of whitespaces */
static int	whitespace_runner(char *input, char *escaped, t_count *c)
{
	if (ft_iswhitespace(input[c->i]) != 1)
	{
		c->count++;
		return (c->i);
	}
	while (ft_iswhitespace(input[c->i]) == 1 && escaped[c->i] == '3')
		c->i++;
	c->count++;
	return (c->i - 1);
}

int	next_char_tester(char *input, char *escaped, t_count *cnt, char c)
{
	if (input[cnt->i + 1] == c && escaped[cnt->i + 1] == 'F')
		cnt->i += 1;
	cnt->i = whitespace_runner(input, escaped, cnt);
	return (cnt->i);
}

int	next_char_runner(char c, char *input, char *escaped, t_count *cnt)
{
	cnt->i++;
	while (input[cnt->i] != c && escaped[cnt->i] != '0')
		cnt->i++;
	cnt->count++;
	return (cnt->i);
}