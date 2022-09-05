/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 20:48:42 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/05 19:53:59 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static int	find_start(t_var *var, int start)
{
	//if (start > 0 && var->t_escape[start] == 'F')
	//	start++;
	while (var->t_input[start])
	{
		if (var->t_input[start] != ' ' && var->t_input[start] != '\t'
			&& var->t_input[start] != '\n' && var->t_escape[start] == '3')
			return (start);
		else if (var->t_escape[start] == 'F' || var->t_escape[start] == 'W')
			return (start);
		else if (var->t_escape[start] == '0'
			&& (var->t_escape[start + 1] == '2'
			|| var->t_escape[start + 1] == '1'))
			return (start + 1);
		else if (var->t_escape[start] == '0' && var->t_escape[start + 1] == '0')
			return (start);
		start++;
	}
	return (start);
}

static int	find_end(t_var *var, int end)
{
	while (var->t_input[end])
	{
		if ((var->t_input[end] == ' ' || var->t_input[end] == '\t'
			|| var->t_input[end] == '\n') && var->t_escape[end] == '3')
			return (end);
		else if (var->t_escape[end] == 'F')
		{
		/*	if (ft_iswhitespace(var->t_input[end + 1]) == 1)
			{
				return (end + 1);
			}
			else */if (var->t_escape[end + 1] != 'F')
				return (end + 1);
			//	return (end);
		//	else if (var->t_escape[end + 1] == 'F')
		//		;
		}
		else if (var->t_escape[end] == 'W')
		{
			if (var->t_escape[end + 1] != 'W')
				return (end + 1);
		}
		else if (var->t_escape[end] == '2' || var->t_escape[end] == '1')
		{
			if (var->t_escape[end + 1] == '0')
				return (end + 1);
		}
		else if (var->t_escape[end] == '0' && var->t_escape[end + 1] == '0')
		{
			if (var->t_escape[end + 2] == '\0')
				return (end);
			return (end + 1);
		}
		end++;
	}
	return (end);
}

void	str_split(t_var *var, t_cmd *cmd, size_t word_num)
{
	size_t	i;
	int		start;
	int		end;

	if (debug_mode < -3)
	{
		printf("str_split.c/76:\tvar->t_input =\t%s\n", var->t_input);
		printf("str_split.c/77:\tvar->t_escape =\t%s\n", var->t_escape);
		printf("str_split.c/78:\tword_num =\t%zu\n", word_num);
		printf("str_split.c/79:\tcmd->nb_cmds =\t%d\n", cmd->nb_cmds);
	}
	start = 0;
	i = 0;
	while (i < word_num)
	{
		start = find_start(var, start);
		if (debug_mode < -3)
			printf("str_split.c/88: start: %d\n", start);
		end	= find_end(var, start);
		if (debug_mode < -3)
			printf("str_split.c/91: end: %d\n", end);
		if (var->t_escape[start] == '0' && var->t_escape[end] == '0'
			&& var->t_escape[end + 1] == '3' && (var->t_input[end + 1] == ' '
				|| var->t_input[end + 1] == '\n' 
				|| var->t_input[end + 1] == '\t'
				|| var->t_input [end + 1] == '\0'))
		{
			if (debug_mode < -3)
				printf("str_split.c/99: str_dup\n");
			cmd->cmd[i] = ft_strdup("");
			cmd->cmd_esc[i] = ft_strdup("00");
		}
		else if (var->t_escape[start] == '0' && var->t_escape[end] == '0'
			&& var->t_escape[end + 1] == '\0')
		{
			cmd->cmd[i] = ft_strdup("");
			cmd->cmd_esc[i] = ft_strdup("00");
		}
		else
		{
			if (var->t_escape[start] == '0' && var->t_escape[end] == '0')
			{
				if (debug_mode < -3)
					printf("str_split.c/114: empty quotes\n");
				start = find_start(var, start + 2);
				end	= find_end(var, start);
				if (debug_mode < -3)
					printf("str_split.c/118: start: %d\n", start);
				if (debug_mode < -3)
					printf("str_split.c/120: end: %d\n", end);
			}
			cmd->cmd[i] = (char *) ft_calloc((end - start + 1), sizeof(char));
			cmd->cmd_esc[i] = (char *) ft_calloc((end - start + 1), sizeof(char));
			if (!cmd->cmd[i] || !cmd->cmd_esc[i])
				return ;
			if (start == end)
			{
				ft_strjoin_char(cmd->cmd[i], var->t_input[start]);
			}
			ft_strlcpy(cmd->cmd[i], &var->t_input[start], end - start + 1);
			ft_strlcpy(cmd->cmd_esc[i], &var->t_escape[start], end - start + 1);
			if (debug_mode < -3)
				printf("str_split.c/129: cmd->cmd[%zu]: \t%s\n", i, cmd->cmd[i]);
			if (debug_mode < -3)
				printf("str_split.c/131: cmd->cmd_esc[%zu]: \t%s\n", i, cmd->cmd_esc[i]);
		}
		start = end;
		i++;
	}
}