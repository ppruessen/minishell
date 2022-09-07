/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 14:33:51 by mschiman          #+#    #+#             */
/*   Updated: 2021/06/12 11:34:26 by mschiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*ptr;
	t_list	*tmp_ptr;

	if (lst && del)
	{
		ptr = *lst;
		while (ptr != NULL)
		{
			tmp_ptr = ptr->next;
			ft_lstdelone(ptr, del);
			ptr = tmp_ptr;
		}
		*lst = NULL;
	}
}
