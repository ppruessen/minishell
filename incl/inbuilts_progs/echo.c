/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:43:48 by pprussen          #+#    #+#             */
/*   Updated: 2022/05/29 13:59:14 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Checks if a string is -n or -nnn... returns 1 if so and 0 if not */
static int	find_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (1);
		else
			return (0);
	}
	return (0);
}

/* Writes the string to the STDOUT with a space between.*/
static void	put_echo_str(int argc, char **argv, int i)
{
	while (argv[i] != NULL)
	{
		if (i == (argc - 1))
			ft_putstr_fd(argv[i], 1);
		else
		{
			ft_putstr_fd(argv[i], 1);
			write (1, " ", 1);
		}
		i++;
	}
}

/* Mimics the original Echo function incl. the -n option. */
int	main(int argc, char **argv, char **env)
{
	int	newline;
	int	i;

	(void)env;
	i = 1;
	if (argc == 1)
		return (-42);
	newline = 1;
	while (argv[i] != NULL)
	{
		if (find_n(argv[i]) == 1)
			newline = 0;
		else
		{
			put_echo_str(argc, argv, i);
			break ;
		}
		i++;
	}
	if (newline == 1)
		write(1, "\n", 1);
	return (0);
}
