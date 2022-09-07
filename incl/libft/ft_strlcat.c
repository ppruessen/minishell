/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 14:04:01 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/05 11:26:23 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;
	size_t	dst_len;
	size_t	app_max;

	src_len = ft_strlen(src);
	dst_len = ft_strlen(dst);
	if (size == 0)
		return (src_len);
	app_max = size - dst_len - 1;
	i = 0;
	if (size <= dst_len)
		return (size + src_len);
	while (src[i] != '\0' && i < app_max)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (i > 0 || i == app_max)
	{
		dst[dst_len + i] = '\0';
		return (src_len + dst_len);
	}
	return (dst_len);
}
