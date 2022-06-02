/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 12:50:04 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/29 14:00:13 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(void)
{
	char	pwd[500];

	ft_bzero(pwd, 500);
	getcwd(pwd, 500);
	printf("%s\n", pwd);
	return (0);
}
