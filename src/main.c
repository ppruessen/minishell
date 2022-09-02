/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:40:13 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/02 19:23:19 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

//TODO ls -la | grep mel | grep file | grep Make >>file68 | ls
// grep mel << stop | grep ist
// Reihenfolge bei redirections kann beliebig sein, lösen!
//bei mehreren >outfiles werden alle dateien erstellt und dann in die letzte geschrieben
//bei mehreren <inputfiles wird nur vom letzten geöffnet und gelesen
//wenn es ein input file nicht gibt wird direkt abgebrochen
// wie lösen wir das in dem codierten input string?

//	fixed: <  <"Makefile" grep main ---->   zsh: parse error near `<'
//			Fehlermeldung eventuell noch anpassen, aber meiner Meinung nach hat das keine Priorität
// <"Makefile" grep main crasht
// <<< Makefile grep main ----> neue Zeile ohne Fehlermeldung
// "<" Makefile grep main ----> segmentation fault   ERLEDIGT
//			bash Ausgabe: "bash: <: command not found"
// "<" wird in box gepackt, aber es gibt am Ende der CMD-Liste keine NULL, da count_words die "<" nicht mitzählt
// die leeren "" partly solved: str_split Zeile 45 return "" statt NULL und set_cmd path check
// Achtung, die leeren "" müssen auch in pipes funktionieren -> sollte laufen?!
// cat | cat | ls läuft nicht wie in bash
// leaks bei str D oder exit ?
// echo $? ganz am Anfang gibt leider -42 aus
// Die "" funktionieren als erstes Argument leider nicht, wie kann man das abfangen?
// sonst zb bei ls "" funktioniert es
// Exit stati von strg D oder strg C sind nicht wie in den bash
// Nach einem command not found muss man manchmal zwei mal exit eingeben
// unexpected token funktionieren nicht mehr -> Syntax Error nochmal checken

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
		g_status = 130; // strg C hat exit status 1 in der bash, Strg D hat 127
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
//			accurat_var_cleaner(&var);
//			accurat_env_cleaner(&var);
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
