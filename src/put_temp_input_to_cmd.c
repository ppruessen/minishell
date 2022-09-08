/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_temp_input_to_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 14:22:03 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 14:42:25 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	word_runner(char *input, char *escaped, t_count *c)
{
	while (ft_iswhitespace(input[c->i]) == 0 && input[c->i] != '|'
		&& input[c->i] != '<' && input[c->i] != '>'
		&& (escaped[c->i] == '3' || escaped[c->i] == 'F'
			|| escaped[c->i] == 'W'))
		c->i++;
	c->count++;
	return (c->i - 1);
}

/* Function counts substrings that are devided by valid whitespaces
// or valid special operators */
static int	count_words(char *input, char *escaped)
{
	t_count	c;

	c.i = 0;
	c.count = 0;
	if (ft_strlen(input) == 0)
		return (0);
	while (input[c.i] != '\0')
	{
		if (input[c.i] == '<' && escaped[c.i] == 'F')
			c.i = next_char_tester(input, escaped, &c, '<');
		else if (input[c.i] == '>' && escaped[c.i] == 'F')
			c.i = next_char_tester(input, escaped, &c, '>');
		else if (input[c.i] != ' ' && input[c.i] != '\t' && input[c.i] != '\n'
			&& input[c.i] != '<' && input[c.i] != '>' && input[c.i] != '|'
			&& (escaped[c.i] == '3' || escaped[c.i] == 'F'
				|| escaped[c.i] == 'W'))
			c.i = word_runner(input, escaped, &c);
		else if (input[c.i] == '"' && escaped[c.i] == '0')
			c.i = next_char_runner('"', input, escaped, &c);
		else if (input[c.i] == '\'' && escaped[c.i] == '0')
			c.i = next_char_runner('\'', input, escaped, &c);
		c.i++;
	}
	return (c.count);
}

void	put_temp_input_to_cmd(t_var *var, t_cmd *cmd)
{
	size_t	word_num;

	word_num = count_words(var->t_input, var->t_escape);
	if (word_num == 0 && cmd->read_from_file == 0 && cmd->write_to_file == 0)
	{
		var->cmds = NULL;
		g_status = 0;
		return ;
	}
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
}
