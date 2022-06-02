/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 13:17:41 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/12 10:03:50 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(s);
	new_str = (char *) ft_calloc((len + 1), sizeof(char));
	if (new_str == NULL)
		return (NULL);
	ft_memcpy(new_str, s, len);
	return (new_str);
}
