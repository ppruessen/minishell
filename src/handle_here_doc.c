/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 15:39:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 14:49:47 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	free_line_and_limiter(char *line, char *limiter)
{
	free(line);
	line = NULL;
	free(limiter);
	limiter = NULL;
}

static	int	keyword_finder(char *limiter, char *line)
{
	if (ft_strlen(limiter) == ft_strlen(line) - 1
		&& ft_strncmp(limiter, line, ft_strlen(limiter)) == 0)
		return (TRUE);
	return (FALSE);
}

static void	get_next_line_minishell(t_cmd *cmd)
{
	char	*line;
	char	rd_char[2];
	int		found_nl;
	int		found_keyword;

	found_keyword = 0;
	ft_bzero(rd_char, 2);
	while (found_keyword == 0)
	{
		line = NULL;
		found_nl = 0;
		while (found_nl == 0)
		{
			if ((read(STDIN_FILENO, &rd_char[0], 1)) != 1)
				break ;
			if (rd_char[0] == '\n')
				found_nl = 1;
			line = ft_strjoin_char(line, rd_char[0]);
		}
		if (keyword_finder(cmd->limiter, line) == TRUE)
			found_keyword = 1;
		else
			write(cmd->fd_to_read, line, ft_strlen(line));
	}
	free_line_and_limiter(line, cmd->limiter);
}

void	handle_here_doc(t_cmd *cmd)
{
	if (cmd->read_from_file == FROM_HERE_DOC)
		get_next_line_minishell(cmd);
}
