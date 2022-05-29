/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:14:12 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/28 23:32:57 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

static size_t	scan_for_nl(char *buff, size_t n, size_t *idx)
{
	*idx = 0;
	if (!buff)
		return (0);
	while (buff[*idx] && n--)
	{
		if (buff[*idx] == '\n')
			return (1);
		(*idx)++;
	}
	return (0);
}

static char	*manage_eof(t_dlst **rems, t_dlst **fd_e, t_dlst **chks, size_t clr)
{
	char	*line;

	if (clr == E_MLC)
	{
		join_clear_list(NULL, rems);
		join_clear_list(NULL, chks);
		*fd_e = NULL;
		return (NULL);
	}
	line = NULL;
	if (chks && *chks)
		line = gather_line(chks);
	if (!(*rems)->next || !(*rems)->next->next)
		join_clear_list(NULL, rems);
	else
	{
		(*fd_e)->prev->next = (*fd_e)->next;
		if ((*fd_e)->next)
			(*fd_e)->next->prev = (*fd_e)->prev;
		malloc_free_p(0, (void **)&((*fd_e)->str));
		malloc_free_p(0, (void **)fd_e);
	}
	return (line);
}

static char	*rec_liner(t_dlst **rems, t_dlst **fd_e, t_dlst **chks, size_t last)
{
	size_t	n_chrs;
	size_t	idx;
	size_t	rm;
	size_t	nl_found;
	t_dlst	*elem;

	n_chrs = read((*fd_e)->n, (*rems)->str, BUFFER_SIZE);
	if (!n_chrs || n_chrs == E_IFD)
		return (manage_eof(rems, fd_e, chks, 0));
	nl_found = scan_for_nl((*rems)->str, n_chrs, &idx);
	idx += nl_found;
	rm = n_chrs - idx;
	if ((last && !ft_substr((*rems)->str, 0, n_chrs, &((*fd_e)->str)))
		|| (!last && !dlst_insert(chks, &elem, NULL, idx))
		|| (!last && !ft_substr((*rems)->str, 0, idx, &(elem->str)))
		|| (!last && !ft_substr((*rems)->str, idx, rm, &((*fd_e)->str))))
		return (manage_eof(rems, fd_e, chks, E_MLC));
	*chks = elem;
	if (last || (nl_found && rm))
		return (gather_line(chks));
	return (rec_liner(rems, fd_e, chks, nl_found));
}

static char	*gnl_prep(t_dlst **rems, t_dlst **fd_e, t_dlst **chks, size_t fd)
{
	size_t	idx;
	size_t	nl_found;
	char	*line;

	while ((*fd_e)->next && (*fd_e)->next->n != fd)
		*fd_e = (*fd_e)->next;
	*fd_e = (*fd_e)->next;
	if (!(*fd_e) && !dlst_insert(rems, fd_e, NULL, SIZE_MAX))
		return (manage_eof(rems, fd_e, chks, E_MLC));
	(*fd_e)->n = fd;
	nl_found = scan_for_nl((*fd_e)->str, SIZE_MAX, &idx);
	idx += nl_found;
	if (!(*fd_e)->str || !nl_found || (nl_found && (*fd_e)->str[idx] == '\0'))
	{
		if ((*fd_e)->str && !dlst_insert(NULL, chks, (*fd_e)->str, idx))
			return (manage_eof(rems, fd_e, chks, E_MLC));
		(*fd_e)->str = NULL;
		return (rec_liner(rems, fd_e, chks, nl_found));
	}
	if (!ft_substr((*fd_e)->str, 0, idx, &line)
		|| !ft_substr((*fd_e)->str, idx, SIZE_MAX, &((*fd_e)->str)))
		return (manage_eof(rems, fd_e, chks, E_MLC));
	return (line);
}

char	*get_next_line(int fd)
{
	static t_dlst	*rems;
	t_dlst			*fd_elem;
	char			*line;
	t_dlst			*chks;

	chks = NULL;
	line = NULL;
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if ((!rems && !dlst_insert(NULL, &rems, NULL, 0))
		|| (!rems->str && !malloc_free_p(sizeof(char) * BUFFER_SIZE,
				(void **)&(rems->str))))
		return (manage_eof(&rems, &fd_elem, &chks, E_MLC));
	fd_elem = rems;
	line = gnl_prep(&rems, &fd_elem, &chks, fd);
	if (line == (char *)E_MLC)
		return (manage_eof(&rems, &fd_elem, &chks, E_MLC));
	return (line);
}
