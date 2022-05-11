/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:15:10 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/11 16:22:23 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	malloc_free_p(size_t size, void **ret_ptr)
{
	if (size < 2)
	{
		free(*ret_ptr);
		*ret_ptr = NULL;
		return (size);
	}
	else
	{
		*ret_ptr = malloc(size);
		if (!(*ret_ptr))
			return (0);
	}
	return (1);
}

int	ft_substr(const char *str, size_t start, size_t n, char **ret)
{
	char	*r;

	if (!str)
		return (0);
	str += start;
	if (!malloc_free_p(sizeof(char) * (n + 1), (void **)ret))
		return (0);
	r = *ret;
	while (n--)
		*(r++) = *(str++);
	*r = '\0';
	return (1);
}

// push_app determines weither the new element is pushed behind dlst (1)
// or appended after dlst (2). push_app = 0 only creates new initialized 
// element at *elem pointer.
int	dlst_insert(t_dlst **dlst, t_dlst **elem, char *str, int psh_app)
{
	if ((psh_app != 2 && !str) || !malloc_free_p(sizeof(t_dlst), (void **)elem))
		return (malloc_free_p(0, (void **)&str));
	(*elem)->prev = NULL;
	(*elem)->next = NULL;
	(*elem)->str = str;
	if (dlst && psh_app == 1)
	{
		if (*dlst)
		{
			if ((*dlst)->prev)
				(*dlst)->prev->next = *elem;
			(*elem)->prev = (*dlst)->prev;
			(*dlst)->prev = *elem;
			(*elem)->next = *dlst;
		}
	}
	else if (dlst && psh_app == 2)
	{
		if ((*dlst)->next)
			(*dlst)->next->prev = *elem;
		(*elem)->next = (*dlst)->next;
		(*dlst)->next = *elem;
		(*elem)->prev = (*dlst);
	}
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
		return (malloc_free_p(1, (void **)chks));
	}
	total_len = elem->n;
	while (elem->next)
	{
		elem = elem->next;
		total_len += elem->n;
	}
	if (!malloc_free_p(sizeof(char) * (total_len + 1), (void **)ret_line))
	{
		*n_chrs = E_MLC;
		return (join_clear_list(NULL, chks, 0));
	}
	(*ret_line)[total_len] = '\0';
	return (join_clear_list(*ret_line, &elem, 1));
}
