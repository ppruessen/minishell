/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:44:56 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/06 17:00:50 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Prints error messages about features which are not supported yet*/
static void	print_error_not_available(t_var *var, int error_code)
{
	if (error_code == BRACES)
		printf("This shell cannot handle braces ...yet.\n");
	else if (error_code == ASTERISK)
		printf("This shell cannot handle asterisks ...yet.\n");
	else if (error_code == DOUBLE_PIPES)
		printf("This shell cannot handle double pipes ...yet.\n");
	else if (error_code == SEMICOLON)
		printf("This shell cannot handle semicolons ...yet.\n");
	else if (error_code == AMPERSAND)
		printf("This shell cannot handle ampersand ...yet.\n");
	else if (error_code == OPEN_QUOTES)
		printf("This shell cannot handle open quotes ...yet.\n");
	else if (error_code == EXCLAMATION)
		printf("This shell cannot handle exclamation marks ...yet.\n");
	if (error_code < 10)
	{
		g_status = 0;
		var->cmd_check = FALSE;
	}
}

/* Prints unexpected syntax near token messages. */
void	print_error(t_var *var, int error_code)
{
	if (error_code == TOO_MANY_REDIR_IN_ONE)
		printf("bash: syntax error near unexpected token `<'\n");
	if (error_code == TOO_MANY_REDIR_IN_TWO)
		printf("bash: syntax error near unexpected token `<<'\n");
	else if (error_code == TOO_MANY_REDIR_OUT_ONE)
		printf("bash: syntax error near unexpected token `>'\n");
	else if (error_code == TOO_MANY_REDIR_OUT_TWO)
		printf("bash: syntax error near unexpected token `>>'\n");
	else if (error_code == UNEXPECTED_TOKEN_NL)
		printf("bash: syntax error near unexpected token `newline'\n");
	else if (error_code == UNEXPECTED_TOKEN_PIPE)
		printf("bash: syntax error near unexpected token `|'\n");
	else
		print_error_not_available(var, error_code);
	if (error_code >= 10)
	{
		g_status = 258;
		var->cmd_check = FALSE;
	}
}

void	print_file_error(t_var *var, int error_code)
{
	if (error_code == FILE_NOT_FOUND)
		printf("No such file or directory\n");
	g_status = 1;
	var->cmd_check = FALSE;
}

void	print_cmd_error(t_var *var, int error_code, char *cmd)
{
	if (error_code == CMD_NOT_FOUND)
	{
		printf("bash: %s: command not found\n", cmd);
		g_status = 127;
		var->cmd_check = FALSE;
	}
}
