/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:15:10 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/23 23:58:11 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

#include <stdio.h>

int	malloc_free_p(size_t size, void **ret_ptr)
{
	if (!size)
	{
//		printf("free : freeing ptr %p\n", *ret_ptr);
		free(*ret_ptr);
		*ret_ptr = NULL;
		return (1);
	}
	else
	{
//		printf("malloc : pre  ret_ptr %p, *ret_ptr %p\n", ret_ptr, *ret_ptr);
		*ret_ptr = malloc(size);
		if (!(*ret_ptr))
			return (0);
//		printf("malloc : post ret_ptr %p, *ret_ptr %p\n", ret_ptr, *ret_ptr);
	}
	return (1);
}

int	ft_substr(char *str, size_t n, char **ret, size_t *n_chrs)
{
	char	*r;
	char	*s;

	printf("sub : start\n");
	if (!str)
		return (0);
	r = str;
	if (n == SIZE_MAX)
		while ((++n < SIZE_MAX) && *r)
			r++;
	if (n)
	{
		if (!malloc_free_p(sizeof(char) * (n + 1), (void **)ret))
			return (0);
		r = *ret;
		s = str;
		while (n--)
			*(r++) = *(s++);
		*r = '\0';
		if (n_chrs)
			*n_chrs = s - str;
	}
	if (n == SIZE_MAX)
	{
		printf("sub : freeing str at %p\n", str);
		malloc_free_p(0, (void **)&str);
	}
	return (1);
}

// psh_app determines weither the new element is pushed behind dlst (1)
// or appended after dlst (2). (0) only creates new initialized 
// element at *elem pointer.
int	dlst_insert(t_dlst **dlst, t_dlst **elem, char *str, int psh_app)
{
//	printf("dlst insertion \n");
	if (!malloc_free_p(sizeof(t_dlst), (void **)elem))
		return (0);
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

int	join_clear_list(char *line, t_dlst **elem)
{
	char	*s;

	printf("join : start\n");
	if (!(*elem))
		return (1);
	while ((*elem)->next)
		*elem = (*elem)->next;
	while (1)
	{
		s = (*elem)->str;
		if (line && s)
			while (*s)
				*(line++) = *(s++);
		printf("join : freeing elem str at ptr %p\n", (*elem)->str);
		malloc_free_p(0, (void **)&((*elem)->str));
		printf("join : elem str freed : %p\n", (*elem)->str);

		if (!((*elem)->prev))
			break ;
		*elem = (*elem)->prev;
		malloc_free_p(0, (void **)&((*elem)->next));
	}
	malloc_free_p(0, (void **)elem);
	return (1);
}

char	*gather_line(t_dlst **chks)
{
	size_t	total_len;
	t_dlst	*elem;
	char	*line;

//	printf("gather : start\n");
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
//	printf("gather : chunk len %zu\n", elem->n);
	while (elem->next)
	{
		elem = elem->next;
		total_len += elem->n;
//		printf("gather : chunk len %zu\n", elem->n);
	}
	if (!malloc_free_p(sizeof(char) * (total_len + 1), (void **)(&line))
		|| !join_clear_list(line, &elem))
		return ((char *)E_MLC);
	line[total_len] = '\0';
	return (line);
}
