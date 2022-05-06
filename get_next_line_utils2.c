/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:15:10 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/05 22:54:51 by iamongeo         ###   ########.fr       */
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
	size_t	len;
	char	*r;

	if (!str)
		return (0);
	str += start;
	len = 0;
	while (str[len] && len < n)
		len++;
	if (!malloc_p(sizeof(char) * (len + 1), (void **)ret))
		return (0);
	r = *ret;
	while (*str && n--)
		*(r++) = *(str++);
	*r = '\0';
	return (1);
}

int	dlst_push_substr(t_dlst **dlst, const char *str, size_t start, size_t len)
{
	t_dlst	*elem;

	if (!malloc_p(sizeof(t_dlst), (void **)&elem))
		return (0);
	elem->str = NULL;
	if (!str && len > 0 && !malloc_p(sizeof(char) * len, (void **)&(elem->str)))
		return (0);
	else if (str && !ft_substr(str, start, len, &elem->str))
		return (0);
	elem->prev = NULL;
	elem->next = NULL;
	elem->n = len;
	if (start == FT_SIZE_MAX)
	{
		elem->prev = *dlst;
		(*dlst)->next = elem;
		return (1);
	}
	else if (*dlst)
	{
		(*dlst)->prev = elem;
		elem->next = *dlst;
	}
	*dlst = elem;
	return (1);
}

static void	join_chunks_list(char *line, t_dlst *elem)
{
	char	*s;

	while (1)
	{
		s = elem->str;
		while (*s)
			*(line++) = *(s++);
		free(elem->str);
		if (elem->prev)
		{
			elem = elem->prev;
			free(elem->next);
		}
		else
			break ;
	}
	free(elem);
}

int	gather_line(t_dlst *dlst, char **ret_line)
{
	t_dlst	*elem;
	size_t	total_len;

	elem = dlst;
	if (!elem)
		return (0);
	if (!(elem->next))
	{
		*ret_line = elem->str;
		free(elem);
		return (1);
	}
	total_len = elem->n;
	while (elem->next)
	{
		elem = elem->next;
		total_len += elem->n;
	}
	if (!malloc_p(sizeof(char) * (total_len + 1), (void **)ret_line))
		return (0);
	join_chunks_list(*ret_line, elem);
	return (1);
}
