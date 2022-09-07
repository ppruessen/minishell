/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 12:06:33 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/06 15:21:46 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief checks if in the string given is an equal sign
 * 
 * @param str [char *]
 * @return int if one or more equal signs are found return > 0, else 0
 */
static int	has_equal_sign(char *str)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	while (str[i] != '\0' && str[0] != '=')
	{
		if (str[i] == '=')
			res++;
		i++;
	}
	return (res);
}

/**
 * @brief Replaces variables in env->list if neccessary
 * 
 * @param var [t_var *]
 * @param cmd [t_cmd *]
 * @return int 1 if a variabel was replaced, else 0
 */
static int	repl_env_list(t_var *var, char *cmd)
{
	t_list	*list;
	char	*temp_str;
	int		i;

	list = var->env_list;
	while (list)
	{
		temp_str = (char *)list->content;
		i = 0;
		while (temp_str[i] && cmd[i] && temp_str[i] == cmd[i])
		{
			if (temp_str[i] == '=' && cmd[i] == '=')
			{
				free (list->content);
				list->content = NULL;
				list->content = ft_strdup(cmd);
				g_status = 0;
				return (1);
			}
			i++;
		}
		list = list->next;
	}
	return (0);
}

/**
 * @brief Prints the env->list
 * 
 * @param list [t_list *]
 */
static void	print_list(t_list *list)
{
	while (list)
	{
		printf("declare -x %s\n", (char *)list->content);
		list = list->next;
	}
	g_status = 0;
}

static void	replace_var(t_var *var, char *cmd)
{
	char	*temp;

	if (ft_isdigit(cmd[0]) == 1)
	{
		printf("bash: export: `%s': not a valid identifier\n", cmd);
		g_status = 1;
	}
	else if (repl_env_list(var, cmd) == 0)
	{
		temp = ft_strdup(cmd);
		ft_lstadd_back(&var->env_list, ft_lstnew(temp));
		g_status = 0;
	}
}

/**
 * @brief Puts variables into the env->list
 * 
 * @param var [t_var *]
 * @param cmd [t_cmd *]
 * @param cmd_esc [char **]
 */
void	export_var(t_var *var, char **cmd, char **cmd_esc)
{
	int		i[2];

	if (cmd[1] == NULL)
		print_list(var->env_list);
	i[0] = 1;
	while (cmd[i[0]] != NULL && cmd[i[0]][0] != '\0')
	{
		i[1] = 0;
		while (cmd_esc[i[0]][i[1]] && cmd_esc[i[0]][i[1]] != '0')
			i[1]++;
		if (cmd[i[0]][i[1]] == '\'')
			cmd[i[0]] = replace_str(cmd[i[0]], "'", "");
		if (cmd[i[0]][i[1]] == '"')
			cmd[i[0]] = replace_str(cmd[i[0]], "\"", "");
		if (has_equal_sign(cmd[i[0]]) > 0)
			replace_var(var, cmd[i[0]]);
		else if (has_equal_sign(cmd[i[0]]) == 0
			&& ft_isdigit(cmd[i[0]][0]) == 1)
		{
			printf("bash: export: `%s': not a valid identifier\n", cmd[i[0]]);
			g_status = 1;
		}
		i[0]++;
	}
}
