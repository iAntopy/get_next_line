/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:14:12 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/11 19:10:21 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

char	*clear_all(t_dlst **rems, t_dlst **fd_elem, t_dlst **chks)
{
	if (*rems)
		join_clear_list(NULL, rems, 0);
	if (*chks)
		join_clear_list(NULL, chks, 0);
	*fd_elem = NULL;
	return (NULL);
}

char	*manage_eof(size_t n_chrs, t_dlst **rems, t_dlst **fd_e, t_dlst **chks)
{
	t_dlst	*elem;

	elem = *fd_e;
	if (n_chrs == E_MLC || (!elem->next && !elem->prev->prev))
		return (clear_all(rems, fd_e, chks));
	elem->prev->next = elem->next;
	if (elem->next)
		elem->next->prev = elem->prev;
	free(elem->str);
	malloc_free_p(0, (void **)fd_e);
	return (NULL);
}

size_t	process_buff(t_dlst **chks, const char *buff, size_t n, char **rem)
{
	size_t	idx;
	char	*s;
	t_dlst	*elem;
	size_t	nl_found;

	elem = NULL;
	idx = -1;
	nl_found = 0;
	if (buff && n)
	{
		while (++idx < n)
			if (buff[idx] == '\n')
				break ;
		nl_found = (idx < n);
		idx += nl_found;
		if (!ft_substr(buff, 0, idx, &s) || !dlst_insert(chks, &elem, s, 1))
			return (E_MLC);
		if (n - idx)
			if (!ft_substr(buff, idx, n - idx, rem))
				return (E_MLC);
		*chks = elem;
		(*chks)->n = idx;
	}
	return (nl_found);
}

int	gnl_prep(t_dlst **rems, t_dlst **fd_elem, size_t fd, size_t *n_chrs)
{
	*n_chrs = 0;
	if (!(*rems)
		&& !dlst_insert(NULL, rems, malloc(sizeof(char) * BUFFER_SIZE), 0))
		return (0);
	*fd_elem = *rems;
	while ((*fd_elem)->next && (*fd_elem)->next->n != fd)
		*fd_elem = (*fd_elem)->next;
	if ((*fd_elem)->next)
	{
		*fd_elem = (*fd_elem)->next;
		if ((*fd_elem)->str)
		{
			*n_chrs = ft_substr((*fd_elem)->str, 0, SIZE_MAX, &((*rems)->str));
			malloc_free_p(0, (void **)&((*fd_elem)->str));
		}
	}
	else
	{
		if (!dlst_insert(rems, fd_elem, NULL, 2))
			return (0);
		(*fd_elem)->n = fd;
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_dlst	*rems;
	char			*line;
	t_dlst			*fd_elem;
	t_dlst			*chks;
	size_t			n_chrs;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	chks = NULL;
	line = NULL;
	if (!gnl_prep(&rems, &fd_elem, fd, &n_chrs))
		return (clear_all(&rems, &fd_elem, &chks));
	while (1)
	{
		n_chrs = process_buff(&chks, rems->str, n_chrs, &(fd_elem->str));
		if (n_chrs == E_MLC || (n_chrs && !gather_line(&chks, &line, &n_chrs)))
			return (clear_all(&rems, &fd_elem, &chks));
		else if (n_chrs && line)
			return (line);
		n_chrs = read(fd, rems->str, BUFFER_SIZE);
		if (n_chrs == E_IFD || (!n_chrs && !gather_line(&chks, &line, &n_chrs)))
			return (manage_eof(n_chrs, &rems, &fd_elem, &chks));
		else if (!n_chrs && line)
			return (line);
	}
}
