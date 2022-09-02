/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 12:06:33 by pprussen          #+#    #+#             */
/*   Updated: 2022/08/28 18:25:13 by mschiman         ###   ########.fr       */
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
		if (str[i] == '=' && ft_isalpha(str[i - 1]) == 1)
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
		while (temp_str[i] == cmd[i])
		{
			if (temp_str[i] == '=' && cmd[i] == '=')
			{
				list->content = ft_strdup(cmd);
				free(temp_str);
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
	while (cmd[i] != NULL)
	{
		if (has_equal_sign(cmd[i]) > 0)
		{
			if (repl_env_list(var, cmd[i]) == 0)
			{
				ft_lstadd_back(&var->env_list, ft_lstnew(cmd[i]));
				g_status = 0;
			}
			if (cmd[i + 1] != NULL && has_equal_sign(cmd[i + 1]) == 0
				&& ft_isdigit(cmd[i + 1][0]) == 1)
			{
				printf("bash: export: `%s': not a valid identifier\n", cmd[i + 1]);
				g_status = 1;
			}
		}
		i++;
	}
}
