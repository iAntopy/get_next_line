/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:15:10 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/11 18:43:19 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

#include <stdio.h>

int	malloc_free_p(size_t size, void **ret_ptr)
{
	if (!size)
	{
		free(*ret_ptr);
		*ret_ptr = NULL;
		return (1);
	}
	else
	{
		*ret_ptr = malloc(size);
		if (!(*ret_ptr))
			return (0);
	}
	return (1);
}

int	ft_substr(char *str, size_t start, size_t n, char **ret)
{
	char	*r;
	char	*s;
	int		sub_to_end;

	if (!str)
		return (0);
	sub_to_end = (n == SIZE_MAX);
	r = str + start;
	if (sub_to_end)
		while ((++n >= 0) && *(r++))
			continue ;
	if (n)
	{
		if (!malloc_free_p(sizeof(char) * (n + 1), (void **)ret))
			return (0);
		r = *ret;
		s = str + start;
		while (n--)
			*(r++) = *(s++);
		*r = '\0';
	}
	if (sub_to_end)
		malloc_free_p(0, (void **)&str);
	return (1);
}
/*
//		if (!malloc_free_p(sizeof(char) * (n + 1)), (void **)ret)
//			return (0);
		(*ret)[n] = '\0';
		while (--n)
			(*ret)[n] = str[n];
		malloc_free_p(0, (void **)&str);
	}
	else if (n != 0)
	{
		if (!malloc_free_p(sizeof(char) * (n + 1), (void **)ret))
			return (0);
		r = *ret;
		while (n--)
			*(r++) = *(str++);
		*r = '\0';
	}
	return (1);
}
*/

// psh_app determines weither the new element is pushed behind dlst (1)
// or appended after dlst (2). (0) only creates new initialized 
// element at *elem pointer.
int	dlst_insert(t_dlst **dlst, t_dlst **elem, char *str, int psh_app)
{
	printf("dlst insertion \n");
	if (!malloc_free_p(sizeof(t_dlst), (void **)elem))
		return (0);//(malloc_free_p(0, (void **)&str));
	(*elem)->prev = NULL;
	(*elem)->next = NULL;
	(*elem)->str = str;
	if (dlst && *dlst && psh_app == 1)
	{
		if ((*dlst)->prev)
			(*dlst)->prev->next = *elem;
		(*elem)->prev = (*dlst)->prev;
		(*dlst)->prev = *elem;
		(*elem)->next = *dlst;
	}
	else if (dlst && *dlst && psh_app == 2)
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
        malloc_free_p(0, (void **)&((*elem)->str));
		if (!((*elem)->prev))
            break ;
        *elem = (*elem)->prev;
        malloc_free_p(0, (void **)&((*elem)->next));
	}
    malloc_free_p(0, (void **)elem);
    return (do_join);
}

char	*gather_line(t_dlst **chks)//, char **ret_line)
{
	size_t	total_len;
	t_dlst	*elem;
	char	*line;

	elem = *chks;
	if (!elem)
		return (NULL);
	if (!(elem->next))
	{
		line = elem->str;
		malloc_free_p(0, (void **)chks);
		return (line);
	}
	total_len = elem->n;
	while (elem->next)
	{
		elem = elem->next;
		total_len += elem->n;
	}
	if (!malloc_free_p(sizeof(char) * (total_len + 1), (void **)(&line))
		|| !join_clear_list(line, &elem, 1))//*ret_line, &elem, 1));
		return ((char *)E_MLC);
	line[total_len] = '\0';
	return (line);
}
