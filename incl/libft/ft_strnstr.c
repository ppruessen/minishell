/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/12 20:24:06 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/14 17:58:58 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (little[0] == '\0' || !little)
		return ((char *) big);
	if (len != 0)
	{
		while (big[i] != '\0' && little[j] != '\0' && i < len)
		{
			j = 0;
			while (big[i + j] == little[j] && (i + j) < len)
			{
				if (j == (ft_strlen(little)) - 1)
					return ((char *)(&big[i]));
				j++;
			}
			i++;
		}
	}
	return (NULL);
}
