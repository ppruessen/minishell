/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 11:13:59 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/15 09:20:20 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Counts the number of digits in an integer.*/
static int	ft_count_digits(int nb)
{
	int	counter;

	counter = 0;
	if (nb < 0)
	{
		if (nb == -2147483648)
			return (10);
		nb = nb * -1;
	}
	if (nb < 10)
		return (1);
	if (nb == 10)
		return (2);
	while (nb / 10 != 0)
	{
		nb = nb / 10;
		counter++;
	}
	counter++;
	return (counter);
}

static char	*ft_write_str(char *res, long num, int size)
{
	res[size - 1] = '\0';
	size = size - 2;
	if (num < 0)
	{
		res[0] = '-';
		num = num * -1;
	}
	while (size > 0)
	{
		res[size] = (num % 10) + '0';
		num = num / 10;
		size--;
	}
	if (res[0] != '-')
		res[0] = num + '0';
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		size;
	long	num;

	num = (long) n;
	size = ft_count_digits(n) + 1;
	if (num < 0)
		size++;
	res = (char *) ft_calloc(size, sizeof (char));
	if (res == NULL)
		return (NULL);
	res = ft_write_str(res, num, size);
	return (res);
}
