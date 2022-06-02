/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 15:52:48 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/12 09:57:21 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_words(char const *s, char del)
{
	size_t	counter;
	size_t	i;

	counter = 0;
	i = 0;
	if (s[i] != '\0' && s[i] != del)
	{
		counter++;
		i++;
	}
	while (s[i] != '\0')
	{
		if (s[i] == del)
			i++;
		else
		{
			if (s[i - 1] == del)
				counter++;
			i++;
		}
	}
	return (counter);
}

static size_t	ft_strlen_substr(const char *s, char del, size_t i)
{
	size_t	len;

	len = 0;
	while (s[i] != del && s[i] != '\0')
	{
		i++;
		len++;
	}
	return (len);
}

static char	**ft_create_substr(const char *s, const char c, char **ptr)
{
	size_t	sub_len;
	size_t	s_len;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	s_len = ft_strlen(s);
	while (i < s_len)
	{
		if (s[i] == c)
			i++;
		while (i == 0 || j < ft_count_words(s, c))
		{
			while (s[i] == c)
				i++;
			sub_len = ft_strlen_substr(s, c, i);
			ptr[j] = ft_substr(s, i, sub_len);
			i = i + sub_len;
			j++;
		}
	}
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;
	size_t	word_num;

	if (!s)
		return (NULL);
	word_num = ft_count_words(s, c);
	ptr = (char **) malloc((word_num + 1) * sizeof(char *));
	if (ptr == NULL)
		return (NULL);
	if (word_num > 0)
		ft_create_substr(s, c, ptr);
	ptr[word_num] = NULL;
	return (ptr);
}
