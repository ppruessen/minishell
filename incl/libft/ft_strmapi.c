/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 11:34:49 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/12 18:08:03 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*res;
	size_t	res_len;
	size_t	i;

	if (!(s && f))
		return (NULL);
	res_len = ft_strlen((char *) s);
	res = (char *) ft_calloc ((res_len + 1), sizeof(char));
	if (res == NULL)
		return (0);
	i = 0;
	while (i < res_len)
	{
		res[i] = f(i, s[i]);
		i++;
	}
	return (res);
}
