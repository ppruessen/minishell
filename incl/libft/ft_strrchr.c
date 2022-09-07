/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 18:10:34 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/12 18:29:48 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*res;
	size_t	i;

	i = 0;
	while (*s != '\0')
	{
		if (*s == (char)c)
		{
			res = (char *)s;
			i++;
		}
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	if (i > 0)
		return (res);
	return (0);
}
