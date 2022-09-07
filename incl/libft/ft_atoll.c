/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 10:25:57 by mschiman          #+#    #+#             */
/*   Updated: 2022/05/12 16:21:15 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "limits.h"
#include "stdio.h"

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

int	ft_atoll(const char *nptr, long long *ret_nb)
{
	long long	i;
	long long	sign;

	i = 0;
	*ret_nb = 0;
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
		*ret_nb = (*ret_nb * 10) + (nptr[i] - '0');
		i++;
	}
	*ret_nb = sign * (*ret_nb);
	if (sign > 0 && *ret_nb >= 0 && *ret_nb <= LLONG_MAX)
		return (0);
	else if (sign < 0 && *ret_nb <= 0 && *ret_nb >= LLONG_MIN)
		return (0);
	else
		return (1);
}
