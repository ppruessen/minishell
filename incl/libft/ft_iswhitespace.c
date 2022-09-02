/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iswhitespace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 10:44:10 by mschiman          #+#    #+#             */
/*   Updated: 2022/08/28 18:13:57 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Returns 1 if a char is blank, tab or newline */
int	ft_iswhitespace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	else
		return (0);
}
