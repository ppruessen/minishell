/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_temp_input_to_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 14:22:03 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/02 15:56:23 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function that runs over whitespaces and returns the index of the
// last whitespace in a line of whitespaces */
int	whitespace_runner(char *input, char *escaped, int i)
{
	if (ft_iswhitespace(input[i]) != 1)
		return (i);
	while (ft_iswhitespace(input[i]) == 1 && escaped[i] == '3')
		i++;
	return (i - 1);
}

int	word_runner(char *input, char *escaped, int i)
{
	while(ft_iswhitespace(input[i]) == 0 && input[i] != '|' 
		&& input[i] != '<' && input[i] != '>' 
		&& (escaped[i] == '3' || escaped[i] == 'F' || escaped[i] == 'W'))
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
		if (input[i] == '<' && escaped[i] == 'F')
		{
			if (input[i + 1] == '<' && escaped[i + 1] == 'F')
				i += 1;
			i = whitespace_runner(input, escaped, i);
			count++;
		}
		else if (input[i] == '>' && escaped[i] == 'F')
		{
			if (input[i + 1] == '>' && escaped[i + 1] == 'F')
				i += 1;
			count++;
			i = whitespace_runner(input, escaped, i);
		}
		else if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n' &&
			input[i] != '<' && input[i] != '>' && input[i] != '|'
			&& (escaped[i] == '3' || escaped[i] == 'F' || escaped[i] == 'W'))
		{
			i = word_runner(input, escaped, i);
			count++;
		}
		else if (input[i] == '"' && escaped[i] == '0')
		{
			i++;
			while (input[i] != '"' && escaped[i] != '0')
				i++;
			count++;
		}
		else if (input[i] == '\'' && escaped[i] == '0')
		{
			i++;
			while (input[i] != '\'' && escaped[i] != '0')
				i++;
			count++;
		}
		i++;
	}
	return (count);
}

void	put_temp_input_to_cmd(t_var *var, t_cmd *cmd)
{
	size_t	word_num;
//	int		i;

	word_num = count_words(var->t_input, var->t_escape);
	if (debug_mode < -3)
		printf("put_temp_input_to_cmd.c/95: word_num: %zu\n", word_num);
	if (word_num == 0 && cmd->read_from_file == 0 && cmd->write_to_file == 0)
	{
		var->cmds = NULL;
		g_status = 0;
		return ;
	}
	if (debug_mode < -3)
		printf("put_temp_input_to_cmd.c/98 word num = %zu\n", word_num);
	cmd->cmd = (char **) malloc((word_num + 1) * sizeof(char *));
	cmd->cmd_esc = (char **) malloc((word_num + 1) * sizeof(char *));
	if (cmd->cmd == NULL || cmd->cmd_esc == NULL)
		return ;
	else
	{
		cmd->cmd[word_num] = NULL;
		cmd->cmd_esc[word_num] = NULL;
	}
	str_split(var, cmd, word_num);
	//i = 0;
	/*while(cmd->cmd[i])
	{
		printf("i = %i\n", i);
		printf("substrings: '%s'\n", cmd->cmd[i]);
		i++;
	}*/
	if (debug_mode < -3)
		printf("put_temp_input_to_cmd.c/117 end put_temp_input_to_cmd\n");
}