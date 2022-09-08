/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 20:48:42 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/08 10:27:59 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	empty_str_cpy(t_cmd *cmd, int i)
{
	cmd->cmd[i] = ft_strdup("");
	cmd->cmd_esc[i] = ft_strdup("00");
}

static int	empty_str_check(t_var *var, int start, int end)
{
	if (var->t_escape[start] == '0' && var->t_escape[end] == '0'
		&& var->t_escape[end + 1] == '3' && (var->t_input[end + 1] == ' '
			|| var->t_input[end + 1] == '\n'
			|| var->t_input[end + 1] == '\t'
			|| var->t_input [end + 1] == '\0'))
		return (TRUE);
	else if (var->t_escape[start] == '0' && var->t_escape[end] == '0'
		&& var->t_escape[end + 1] == '\0')
		return (TRUE);
	return (FALSE);
}

static int	create_cmd(t_cmd *cmd, int i, int start, int end)
{
	cmd->cmd[i] = (char *) ft_calloc((end - start + 1), sizeof(char));
	cmd->cmd_esc[i] = (char *) ft_calloc((end - start + 1), sizeof(char));
	if (!cmd->cmd[i] || !cmd->cmd_esc[i])
		return (FALSE);
	return (TRUE);
}

static void	find_start_and_end(t_var *var, int start, t_split *split)
{
	split->start = find_start(var, start);
	split->end = find_end(var, split->start);
}

void	str_split(t_var *var, t_cmd *cmd, size_t word_num)
{
	size_t	i;
	t_split	s;

	s.start = 0;
	i = 0;
	while (i < word_num)
	{
		find_start_and_end(var, s.start, &s);
		if (empty_str_check(var, s.start, s.end) == TRUE)
			empty_str_cpy(cmd, i);
		else
		{
			if (var->t_escape[s.start] == '0' && var->t_escape[s.end] == '0')
				find_start_and_end(var, s.start + 2, &s);
			if (create_cmd(cmd, i, s.start, s.end) == FALSE)
				return ;
			ft_strlcpy(cmd->cmd[i], &var->t_input[s.start],
				s.end - s.start + 1);
			ft_strlcpy(cmd->cmd_esc[i], &var->t_escape[s.start],
				s.end - s.start + 1);
		}
		s.start = s.end;
		i++;
	}
}
