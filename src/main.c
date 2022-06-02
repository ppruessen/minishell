/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:40:13 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/30 18:37:48 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

//TODO ls -la | grep mel | grep file | grep Make >>file68 | ls
// grep mel << stop | grep ist

/* Initialise the var-struct variables */
static void init_var(t_var *var, int argc, 
	char **argv, char **env)
{
	var->argc = argc;
	var->argv = argv;
	var->env = env;
	copy_env(var, env);
	ft_bzero(var->pwd, 500);
	getcwd(var->pwd, 500);
	var->input = NULL;
	var->input_escape = NULL;
	var->prompt = ft_strdup(PAMELA_PROMPT);
}

/* Transforming the SIGINT signal to a newline in the prompt */
static void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_status = 130;
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
	while (1)
	{
		g_status = -42;
		var.input = readline(var.prompt);
		if (!var.input)
		{
			write(1, "Zeile 59 raus gesprungen.\n", 27);
			exit(g_status);
		}
		// check for quotes; if quotes open (Fehlermeldung und Ausstieg) -> here_doc
		if (var.input[0] != '\0')
		{
			//while (g_status == -42)
			//{
				add_history(var.input);
				parse_input(&var);
				if (g_status == -42)
					execute_cmds(&var);
			//}
		}
		printf("Input string alt:\t|%s|\n", var.input);
		printf("Exit status:\t %i\n", g_status);
	}
	return (g_status);
}