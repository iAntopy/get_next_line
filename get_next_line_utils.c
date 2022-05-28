/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:15:10 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/28 18:15:28 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

#include <stdio.h>

int	malloc_free_p(size_t size, void **ret_ptr)
{
	if (!size && ret_ptr && *ret_ptr)
	{
//		printf("free : freeing ptr %p\n", *ret_ptr);
		free(*ret_ptr);
		*ret_ptr = NULL;
	}
	else if (size > 0)
	{
//		printf("malloc : size %zu\n", size);
//		printf("malloc : pre  ret_ptr %p, *ret_ptr %p\n", ret_ptr, *ret_ptr);
		*ret_ptr = malloc(size);
		if (!(*ret_ptr))
			return (0);
//		printf("malloc : post ret_ptr %p, *ret_ptr %p\n", ret_ptr, *ret_ptr);
	}
	return (1);
}

int	ft_substr(char *str, size_t start, size_t n, char **ret)//, void *ptr)//size_t *n_chrs)
{
	char	*r;
	char	*s;
	int	to_end;

//	printf("sub : subbing %zu chars of str %p, at ptr %p\n", n, str, *ret);
	to_end = (n == SIZE_MAX);
	if (!str)
		return (0);
	r = str + start;
	if (to_end)
	{
//		printf("sub : sub to end\n");
		while ((++n < SIZE_MAX) && *r)
			r++;
	}
	if (n)
	{
//		printf("sub : mallocing %zu bytes\n", n + 1);
		if (!malloc_free_p(sizeof(char) * (n + 1), (void **)ret))
			return (0);
		r = *ret;
		s = str + start;
		while (n--)
			*(r++) = *(s++);
		*r = '\0';
	}
	if (to_end)
	{
//		printf("sub : freeing ptr %p\n", str);
		malloc_free_p(0, (void **)&str);
//		printf("sub : freeing over\n");
	}
	return (1);
}

// psh_app determines weither the new element is pushed behind dlst (1)
// or appended after dlst (2). (0) only creates new initialized 
// element at *elem pointer.
int	dlst_insert(t_dlst **dlst, t_dlst **elem, char *str, size_t psh_app)
{
//	printf("dlst insertion, psh_app : %zu\n", psh_app);
	if (!malloc_free_p(sizeof(t_dlst), (void **)elem))
		return (0);
	(*elem)->prev = NULL;
	(*elem)->next = NULL;
	(*elem)->str = str;
	(*elem)->n = psh_app;
	if (dlst && *dlst && psh_app == SIZE_MAX)
	{
		if ((*dlst)->next)
			(*dlst)->next->prev = *elem;
		(*elem)->next = (*dlst)->next;
		(*dlst)->next = *elem;
		(*elem)->prev = (*dlst);
	}
	else if (dlst && *dlst && psh_app > 0)
	{
//		printf("insert : pushing chunk of len %zu\n", psh_app);
		if ((*dlst)->prev)
			(*dlst)->prev->next = *elem;
		(*elem)->prev = (*dlst)->prev;
		(*dlst)->prev = *elem;
		(*elem)->next = *dlst;
	}
	return (1);
}

int	join_clear_list(char *line, t_dlst **elem)
{
	char	*s;

//	printf("join : start\n");
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
//		printf("join : freeing elem str at ptr %p\n", (*elem)->str);
		malloc_free_p(0, (void **)&((*elem)->str));
//		printf("join : elem str freed : %p\n", (*elem)->str);

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
//		printf("gather : returning line from single chunk, line : %s\n", line);
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
