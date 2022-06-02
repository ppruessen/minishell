/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 15:39:08 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/30 17:52:34 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	get_next_line_minishell(t_cmd *cmd)
{
	char	*line;
	char	rd_char[2];
	int		found_nl;
	int		found_keyword;
	char	*limiter;

	if (cmd->write_to_file == 0)
		limiter = cmd->cmd[cmd->nb_cmds - 1];
	else
		limiter = cmd->cmd[cmd->nb_cmds - 2];
	found_keyword = 0;
	ft_bzero(rd_char, 2);
	while (found_keyword == 0)
	{
		line = NULL;
		found_nl = 0;
		while (found_nl == 0)
		{
			printf("I was here Zeile 32.\n");
			if ((read(STDIN_FILENO, &rd_char[0], 1)) != 1)
				break ;
			if (rd_char[0] == '\n')
				found_nl = 1;
			printf("I was here Zeile 37.\n");
			line = ft_strjoin_char(line, rd_char[0]);
		}
		if (ft_strncmp(limiter, line, ft_strlen(limiter)) != 0)
			write(cmd->fd_to_read, line, ft_strlen(line));
		else
			found_keyword = 1;
		printf("I was here Zeile 44.\n");
		free(line);
		free(limiter);
	}
}

void	handle_here_doc(t_cmd *cmd)
{
	if (cmd->read_from_file == 2)
		get_next_line_minishell(cmd);
}