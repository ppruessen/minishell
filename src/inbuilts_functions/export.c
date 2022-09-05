/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 12:06:33 by pprussen          #+#    #+#             */
/*   Updated: 2022/09/05 16:53:11 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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

int	repl_env_list(t_var *var, char *cmd)
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

void	export_var(t_var *var, char **cmd)
{
	int		i;
	t_list	*list;
	char	*temp;

	list = var->env_list;
	if (cmd[1] == NULL)
	{	
		while (list)
		{
			printf("declare -x %s\n", (char *)list->content);
			list = list->next;
		}
		g_status = 0;
	}
	i = 1;
	while (cmd[i] != NULL && cmd[i][0] != '\0') //check ob die zweite condition nicht woanders crasht
	{
		cmd[i] = replace_str(cmd[i], "'", "");
		cmd[i] = replace_str(cmd[i], "\"", "");
		if (has_equal_sign(cmd[i]) > 0)
		{
			if (ft_isdigit(cmd[i][0]) == 1)
			{
				printf("bash: export: `%s': not a valid identifier\n", cmd[i]);
				g_status = 1;
			}
			else if (repl_env_list(var, cmd[i]) == 0)
			{
				temp = ft_strdup(cmd[i]);
				ft_lstadd_back(&var->env_list, ft_lstnew(temp));
				g_status = 0;
			}
		}
		else if (has_equal_sign(cmd[i]) == 0 && ft_isdigit(cmd[i][0]) == 1)
		{
			printf("bash: export: `%s': not a valid identifier\n", cmd[i]);
			g_status = 1;
		}
		i++;
	}
}
