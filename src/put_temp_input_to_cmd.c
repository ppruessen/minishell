/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_temp_input_to_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 14:22:03 by mschiman          #+#    #+#             */
/*   Updated: 2022/06/02 13:47:43 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function that runs over whitespaces and returns the index of the
// last whitespace in a line of whitespaces */
static int	whitespace_runner(char *input, char *escaped, int i)
{
	while (ft_iswhitespace(input[i]) == 1 && escaped[i] == '3')
		i++;
	return (i - 1);
}

static int	word_runner(char *input, char *escaped, int i)
{
	while ((input[i] != ' ' && input[i] != '\t' && input[i] != '\n' &&
			input[i] != '<' && input[i] != '>' && input[i] != '|'
			&& escaped[i] == '3'))
		i++;
	return (i - 1);
}

/* Function counts substrings that are devided by valid whitespaces
// or valid special operators */
static int	count_words(char *input, char *escaped)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (ft_strlen(input) == 0)
		return (0);
	while (input[i] != '\0')
	{
		if (input[i] == '<' && escaped[i] == '3')
		{
			if (input[i + 1] == '<' && escaped[i + 1] == '3')
				i += 1;
			i = whitespace_runner(input, escaped, i);
			count++;
		}
		else if (input[i] == '>' && escaped[i] == '3')
		{
			if (input[i + 1] == '>' && escaped[i + 1] == '3')
				i += 1;
			count++;
			i = whitespace_runner(input, escaped, i);
		}
		else if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n' &&
			input[i] != '<' && input[i] != '>' && input[i] != '|'
			&& escaped[i] == '3')
		{
			i = word_runner(input, escaped, i);
			count++;
		}
		i++;
	}
	return (count);
}

void	put_temp_input_to_cmd(t_var *var, t_cmd *cmd)
{
	size_t	word_num;
	int		i;

	word_num = count_words(var->t_input, var->t_escape);
	printf("word num = %zu\n", word_num);
	if (word_num == 0)
	{
		var->cmds = NULL;
		g_status = 0;
		return ;
	}
	cmd->cmd = (char **) malloc((word_num + 1) * sizeof(char *));
	if (cmd->cmd == NULL)
		return ;
	// split input and checking escaped for valid cuts, stored in cmd->cmd
	printf("word num = %zu\n", word_num);
	cmd->cmd[word_num] = NULL;
	str_split(var, cmd);
	i = 0;
	while(cmd->cmd[i])
	{
		printf("substrings: '%s'\n", cmd->cmd[i]);
		i++;
		printf("i = %i\n", i);
	}
	printf("end put_temp_input_to_cmd\n");
}