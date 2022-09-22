/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:40:13 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/19 12:22:59 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Initialise the var-struct variables */
static void	init_var(t_var *var, int argc, 
	char **argv, char **env)
{
	var->argc = argc;
	var->argv = argv;
	var->env = env;
	var->env_list = NULL;
	var->dir_list = NULL;
	var->path = NULL;
	var->prompt = NULL;
	var->input = NULL;
	var->input_escape = NULL;
	var->t_input = NULL;
	var->t_escape = NULL;
	var->fd = NULL;
	var->cmds = NULL;
	var->dollar_var = NULL;
	var->dollar_esc = NULL;
	var->dollar_value = NULL;
	var->dollar_esc_value = NULL;
	copy_env(var, env);
	ft_bzero(var->pwd, 500);
	getcwd(var->pwd, 500);
	var->prompt = ft_strdup(PAMELA_PROMPT);
}

/* Transforming the SIGINT signal to a newline in the prompt */
static void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_status = 1; // strg C hat exit status 1 in der bash, Strg D hat 127
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
//		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

int	main(int argc, char **argv, char **env)
{
	t_var	var;

	init_var(&var, argc, argv, env);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	g_status = 0;
	while (1)
	{
		var.cmd_check = TRUE;
		var.input = readline(var.prompt);
		if (!var.input)
		{
			accurat_var_cleaner(&var);
			accurat_env_cleaner(&var);
			exit(g_status);
		}
		if (var.input[0] != '\0')
		{
			add_history(var.input);
			parse_input(&var);
			if (var.cmd_check == TRUE)
				execute_cmds(&var);
		}
		accurat_var_cleaner(&var);
	}
	return (g_status);
}
