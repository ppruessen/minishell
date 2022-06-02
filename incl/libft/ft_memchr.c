/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/22 14:16:10 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/12 11:36:30 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr_src;
	size_t			i;

	i = 0;
	ptr_src = (unsigned char *)s;
	while (i < n && n > 0)
	{
		if (ptr_src[i] == (unsigned char)c)
			return (ptr_src + i);
		i++;
	}
	return (0);
}
