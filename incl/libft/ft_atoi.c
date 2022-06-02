/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 10:25:57 by mschiman          #+#    #+#             */
/*   Updated: 2022/01/27 16:58:01 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Checks if a char is + or -. Returns 0 if neither or no input.*/
static int	ft_checksign(char c)
{
	int	sign;

	sign = 0;
	if (c)
	{
		if (c == '-')
			sign = -1;
		else if (c == '+')
			sign = 1;
	}
	return (sign);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	res = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (ft_checksign(nptr[i]) != 0)
	{
		sign = ft_checksign(nptr[i]);
		i++;
	}
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		res = (res * 10) + (nptr[i] - '0');
		i++;
	}
	return (sign * res);
}
