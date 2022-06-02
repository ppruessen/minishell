/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 20:48:42 by pprussen          #+#    #+#             */
/*   Updated: 2022/05/29 13:28:27 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*	split -- split a file into pieces
*	DESCRIPTION
*	Allocates (with malloc(3)) and returns an array
*	of strings obtained by splitting ’s’ using the
*	character ’c’ as a delimiter. The array must be
*	ended by a NULL pointer.
*	RETURN VALUES
*	The array of new strings resulting from the split.
*	NULL if the allocation fails.
*/

#include "../incl/minishell.h"


static char	*str_dup_with_char_extract(t_var *var, size_t i)
{
	char	*new_str;
	size_t	len;
	size_t	start;

	start = i;
	len = 0;
	while (var->t_input[i])
	{
		if (var->t_escape[i] != '0')
			len++;
		i++;
	}
	new_str = (char *) ft_calloc((len + 1), sizeof(char));
	if (new_str == NULL)
		return (NULL);
	len = 0;
	while (var->t_input[start])
	{
		if (var->t_escape[start] != '0')
		{
			new_str[len] = var->t_input[start];
			len++;
		}
		start++;
	}
	return (new_str);
}

static void	array_filler(t_var *var, int str_len_input, t_cmd *cmd)
{
	int	i;
	int	j;
	int	in_word;

	in_word = 0;
	i = 0;
	j = 0;
	while (i < str_len_input)
	{
		if (var->t_input[i] != '\0' && in_word == 0)
		{
			in_word = 1;
			cmd->cmd[j] = str_dup_with_char_extract(var, i);
			printf("cmd->cmd[%d]: %s\n", j, cmd->cmd[j]);
			j++;
		}
		else if (var->t_input[i] == '\0')
		{
			in_word = 0;
		}
		i++;
	}
	cmd->nb_cmds = j;
}

void	create_cut_marks(t_var *var, int str_len_input)
{
	int	i;

	i = 0;
	while (i < str_len_input)
	{
		if (var->t_input[i] == '<' && var->t_escape[i] == '3')
		{
			var->t_input[i] = '\0';
			if (var->t_input[i + 1] == '<' && var->t_escape[i + 1] == '3')
			var->t_input[i + 1] = '\0';
		}
		else if (var->t_input[i] == '>' && var->t_escape[i] == '3')
		{
			var->t_input[i] = '\0';
			if (var->t_input[i + 1] == '>' && var->t_escape[i + 1] == '3')
				var->t_input[i + 1] = '\0';
		}
		else if ((var->t_input[i] == ' ' || var->t_input[i] == '\t' || var->t_input[i] == '\n') 
			&& var->t_escape[i] == '3')
			var->t_input[i] = '\0';
		i++;
	}
}

void	str_split(t_var *var, t_cmd *cmd)
{
	int		str_len_input;

	str_len_input = ft_strlen(var->t_input);
	create_cut_marks(var, str_len_input);
	array_filler(var, str_len_input, cmd);
//	free(input);
//	free(escaped);
}