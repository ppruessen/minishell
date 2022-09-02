/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 15:39:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/08/31 16:45:10 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

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
		if (ft_strlen(cmd->limiter) == ft_strlen(line) - 1
			&& ft_strncmp(cmd->limiter, line, ft_strlen(cmd->limiter)) == 0)
			found_keyword = 1;
		else
			write(cmd->fd_to_read, line, ft_strlen(line));
	}
	free(line);
	free(cmd->limiter);
}

void	handle_here_doc(t_cmd *cmd)
{
	if (cmd->read_from_file == FROM_HERE_DOC)
		get_next_line_minishell(cmd);
}
