/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 16:57:57 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/12 14:38:30 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*res;
	size_t	i;

	res = (char *) malloc(nmemb * size);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < (nmemb * size))
	{
		res[i] = '\0';
		i++;
	}
	return (res);
}
