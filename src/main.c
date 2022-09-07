/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:40:13 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/06 09:54:46 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

//echo $USER$HOME			// Hendrik interpretiert nur den ersten Teil
//<out3<out4 grep main		// kann Hendrik auch nicht
//SRC =   ./src/main.c 
//<  <"Makefile" grep main
//bash: syntax error near unexpected token `<'
// cat | cat | ls läuft nicht wie in bash
//PATH="ls -la export vergisst die schließenden "


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

	if (argc == 2)
		debug_mode = atoi(argv[1]); // REMOVE BEFORE SUBMITTING - more debug-infos for smaler numbers
									// -1 shows leak informations
	else
		debug_mode = 0;

	init_var(&var, argc, argv, env);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	g_status = 0;
	while (1)
	{
		if (debug_mode < -3)
			printf("\nMain.c/83 G_STATUS: %d\n", g_status);
		var.cmd_check = TRUE;
		var.input = readline(var.prompt);
		if (!var.input)
		{
//			clean_env(&var);
//			free (var.prompt);
//			var.prompt = NULL;
			if (debug_mode < -3)
				write(1, "Main.c/89 raus gesprungen.\n", 27);
			accurat_var_cleaner(&var);
			accurat_env_cleaner(&var);
			if (debug_mode < 0)
				accurat_finder(&var);
			if (debug_mode < -1)
				check_leaks();
			exit(g_status);
		}
		if (var.input[0] != '\0')
		{
			add_history(var.input);
			parse_input(&var);
			if (var.cmd_check == TRUE)
				execute_cmds(&var);
		}
		if (debug_mode < -3)
			printf("Main.c/100: Input string alt:\t|%s|\n", var.input);
		if (debug_mode < -3)
			printf("Main.c/102: Exit status:\t %i\n", g_status);
		accurat_var_cleaner(&var);
//		if (debug_mode < 0)
//			accurat_finder(&var);
	}
	return (g_status);
}
