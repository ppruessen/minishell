/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 15:50:03 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/13 09:15:28 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Function creates an absolute path from a relativ path */
char	*create_rel_path(char *cmd_path)
{
	char	pwd[500];
	char	*pwd_temp;
	char	*cmd_path_temp;

	ft_bzero(pwd, 500);
	getcwd(pwd, 500);
	pwd_temp = ft_strdup(pwd);
	cmd_path_temp = ft_strdup(cmd_path);
	free (cmd_path);
	cmd_path = NULL;
	pwd_temp = ft_strjoin_char(pwd_temp, '/');
	cmd_path = ft_strjoin(pwd_temp, cmd_path_temp);
	free (pwd_temp);
	pwd_temp = NULL;
	free (cmd_path_temp);
	cmd_path_temp = NULL;
	return (cmd_path);
}

/* Function to get the PATH Variable from the env_list */
void	get_path(t_var *var)
{
	t_list	*list;

	list = var->env_list;
	while (list)
	{
		if (!ft_strncmp("PATH=", list->content, ft_strlen("PATH=")))
		{
			var->path = ft_strnstr(list->content,
					"PATH=", ft_strlen(list->content));
			var->path = ft_strtrim(var->path, "PATH=");
			break ;
		}
		list = list->next;
	}
	return ;
}

/* Checks if the command is found within the path directories. */
/* Returns the correct path or the cmd if path not found. */
char	*check_and_set_path(char *cmd, t_var *var)
{
	char	*valid_cmd_path;
	int		i;

	get_path(var);
	gen_dir_list(var);
	i = 0;
	while (var->dir_list[i] != NULL)
	{
		if (cmd[0] == '\0')
			break ;
		valid_cmd_path = ft_strjoin(var->dir_list[i], cmd);
		if (access(valid_cmd_path, X_OK) == 0)
		{
			free_dir_list_cmd(var->dir_list, cmd);
			return (valid_cmd_path);
		}
		free (valid_cmd_path);
		i++;
	}
	return (cmd);
}

/* Sets the cmd path to unbuilt functions */
char	*set_inbuilt_path(t_cmd *cmd, t_var *var)
{
	if (cmd->inbuilt == ECHO || cmd->inbuilt == ENV || cmd->inbuilt == PWD)
	{
		free(cmd->cmd[0]);
		cmd->cmd[0] = NULL;
	}
	if (cmd->inbuilt == ECHO)
		return (ft_strjoin(var->pwd, "/incl/inbuilts_progs/echo"));
	else if (cmd->inbuilt == ENV)
		return (ft_strjoin(var->pwd, "/incl/inbuilts_progs/env"));
	else if (cmd->inbuilt == PWD)
		return (ft_strjoin(var->pwd, "/incl/inbuilts_progs/pwd"));
	return (cmd->cmd[0]);
}

/* Sets the cmd path */
void	set_cmd_path(t_cmd *cmd, t_var *var)
{
	int	i;

	i = 0;
	while (cmd->cmd[i] && (cmd->cmd_esc[i][0] == 'F'
		|| cmd->cmd_esc[i][0] == 'W'))
		i++;
	if (cmd->cmd[i] && cmd->inbuilt == 0)
	{
		if (cmd->cmd[i][0] == '.')
			cmd->cmd[i] = create_rel_path(cmd->cmd[i]);
		else if (cmd->cmd[i][0] == '/')
			return ;
		else
			cmd->cmd[i] = check_and_set_path(cmd->cmd[i], var);
	}
	else if (cmd->cmd[i])
		cmd->cmd[i] = set_inbuilt_path(cmd, var);
}
