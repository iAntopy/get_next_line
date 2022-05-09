/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:15:10 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/06 21:14:59 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line2.h"

#include <stdio.h>

int	malloc_p(size_t size, void **ret_ptr)
{
	printf("malloc ing size : %zu\n", size);
	*ret_ptr = malloc(size);
	if (!(*ret_ptr))
		return (0);
	return (1);
}

int	ft_substr(const char *str, size_t start, size_t n, char **ret)
{
	//size_t	len;
	char	*r;
	printf("Substr : substringing starts\n");

	if (!str)
		return (0);
	printf("Substr : str ptr present \n");
	str += start;
	printf("Substr : Pre malloc_p\n");
	if (!malloc_p(sizeof(char) * (n + 1), (void **)ret))
		return (0);
	r = *ret;
	printf("Substr : Entering while copy\n");
	while (n--)
		*(r++) = *(str++);
	*r = '\0';
	printf("Substr : string null terminated and closing.\n");
	return (1);
}

int	dlst_insert(t_dlst **dlst, t_dlst **elem, char *mstr, int push_app)
{
	printf("dlst insert : starting insertion, push_app : %d\n", push_app);
	printf("dlst insert : ptrs elem %p\n", *elem);
	if ((push_app != 2 && !mstr) || !malloc_p(sizeof(t_dlst), (void **)elem))
	{
		printf("dlst insert : trying to free mstr\n");
		free(mstr);
		return (0);
	}
	printf("dlst insert : linking new elem \n");
	(*elem)->prev = NULL;
	(*elem)->next = NULL;
	(*elem)->str = mstr;
	printf("dlst insert : elem initialized\n");
	if (dlst && !(*dlst))
		*dlst = *elem;
	else if (dlst && push_app == 1) //push element before *dlst
	{
		if ((*dlst)->prev)
			(*dlst)->prev->next = *elem;
		(*elem)->prev = (*dlst)->prev;
		(*dlst)->prev = *elem;
		(*elem)->next = *dlst;
	}
	else if (dlst && push_app == 2) //append element after *dlst
	{
		if ((*dlst)->next)
			(*dlst)->next->prev = *elem;
		(*elem)->next = (*dlst)->next;
		(*dlst)->next = *elem;
		(*elem)->prev = (*dlst);
	}
	printf("insert : elem prev, elem next : %p, %p\n", (*elem)->prev, (*elem)->next);
	return (1);
}

int	join_clear_list(char *line, t_dlst **elem, int do_join)
{
	char	*s;

	if (!(*elem))
		return (do_join);
	while ((*elem)->next)
		*elem = (*elem)->next;
	while (1)
	{
		s = (*elem)->str;
		if (do_join && s)
			while (*s)
				*(line++) = *(s++);
		free((*elem)->str);
		if ((*elem)->prev)
		{
			*elem = (*elem)->prev;
			free((*elem)->next);
		}
		else
			break ;
	}
	free(*elem);
	*elem = NULL;
	return (do_join);
}

int	gather_line(t_dlst **chks, char **ret_line, size_t *n_chrs)
{
	size_t	total_len;
	t_dlst	*elem;

	elem = *chks;	
	if (!elem)
		return (0);
	if (!(elem->next))
	{
		*ret_line = elem->str;
		free(elem);
		*chks = NULL;
		return (1);
	}
	total_len = elem->n;
	while (elem->next)
	{
		elem = elem->next;
		total_len += elem->n;
	}
	if (!malloc_p(sizeof(char) * (total_len + 1), (void **)ret_line))
	{
		*n_chrs = -2;
		return (join_clear_list(NULL, chks, 0));
	}
	return (join_clear_list(*ret_line, &elem, 1));
}