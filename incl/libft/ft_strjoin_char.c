/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 16:40:54 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/30 15:46:48 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlen_del(const char *s, char del)
{
	int	i;

	i = 0;
	if (s)
	{
		while (s[i] != del && s[i] != '\0')
			i++;
	}
	return (i);
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*join;
	size_t	join_len;
	size_t	i;

	i = 0;
	if (!c)
		return (ft_strdup(s1));
	if (!s1)
	{
		join = (char *) ft_calloc (2, sizeof (char));
		join[0] = c;
		join[1] = '\0';
		return (join);
	}
	join_len = ft_strlen(s1) + 1;
	join = (char *) ft_calloc ((join_len + 1), sizeof (char));
	if (join == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		join[i] = s1[i];
		i++;
	}
	join[i] = c;
	return (join);
}
