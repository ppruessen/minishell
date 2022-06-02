/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:44:56 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/27 13:40:43 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Prints error messages about features which are not supported yet*/
static void	print_error_not_available(int error_code)
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
		g_status = 42;
}

/* Prints unexpected syntax near token messages. */
void	print_error(int error_code)
{
	if (error_code == TOO_MANY_REDIR_IN)
		printf("bash: syntax error near unexpected token `<<'\n");
	else if (error_code == TOO_MANY_REDIR_OUT)
		printf("bash: syntax error near unexpected token `>>'\n");
	else if (error_code == UNEXPECTED_TOKEN_NL)
		printf("bash: syntax error near unexpected token `newline'\n");
	else if (error_code == UNEXPECTED_TOKEN_PIPE)
		printf("bash: syntax error near unexpected token `|'\n");
	else
		print_error_not_available(error_code);
	if (error_code >= 10)
		g_status = 258;
}
