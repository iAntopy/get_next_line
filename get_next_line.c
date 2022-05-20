/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:14:12 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/11 19:58:04 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

#include <stdio.h>

/*
char	*clear_all(t_dlst **rems, t_dlst **fd_elem, t_dlst **chks)
{
//	printf("THE SITUATION IS CLEAR !!");
    if (*rems)
	    join_clear_list(NULL, rems, 0);
    if (*chks)
	    join_clear_list(NULL, chks, 0);
	if (*fd_elem)
		*fd_elem = NULL;
	return (NULL);
}
*/

size_t	scan_for_nl(char *buff, size_t *idx)
{
	*idx = 0;
	while (buff[*idx])
	{
		if (buff[*idx] == '\n')
			return (1);
		(*idx)++;
	}
	return (0);
}

char	*manage_eof(t_dlst **rems, t_dlst **fd_e, t_dlst **chks, char *line)
{
	t_dlst	*elem;

	printf("Managing eof\n");
	if ((size_t)line == E_MLC)
	{
		printf("managing eof CLEAR ALL !\n");
		join_clear_list(NULL, rems, 0);
		join_clear_list(NULL, chks, 0);
		*fd_e = NULL;
		return (NULL);
	}
	elem = *fd_e;
	if (chks && *chks)
		line = gather_line(chks);
	elem->prev->next = elem->next;
	if (elem->next)
		elem->next->prev = elem->prev;
	malloc_free_p(0, (void **)&((*fd_e)->str));
	malloc_free_p(0, (void **)fd_e);
	if (!((*rems)->next->next))//(!elem->next && !elem->prev->prev)
		join_clear_list(NULL, rems, 0);
	return (line);
}
/*
char	*manage_eof(size_t n_chrs, t_dlst **rems, t_dlst **fd_e, t_dlst **chks)
{
	t_dlst	*elem;

//	printf("Managing EOF\n");
	elem = *fd_e;
	if (n_chrs == E_MLC || (!elem->next && !elem->prev->prev))
		return (clear_all(rems, fd_e, chks));
	elem->prev->next = elem->next;
	if (elem->next)
		elem->next->prev = elem->prev;
	malloc_free_p(0, (void **)&((*fd_e)->str));
	elem = NULL;
	malloc_free_p(0, (void **)fd_e);
	return (NULL);
}
*/
/*
size_t	process_buff(size_t n_chrs, t_dlst **rems, t_dlst **fd_e, t_dlst **chks)//char **rem)
{
	size_t	idx;
	t_dlst	*elem;
	size_t	nl_found;
	char	*buff;

	printf("\n-+-PROCESSING-+-\n");
	printf("n_chrs : %zu\n", n_chrs);
	idx = -1;
	buff = (*rems)->str;
	nl_found = (n_chrs == E_EOF);
	if ((*rems)->str && n_chrs)
	{
		while (++idx < n_chrs)
			if (buff[idx] == '\n')
				break ;
		nl_found = (idx < n_chrs);
		idx += nl_found;
		if (!ft_substr(buff, 0, idx, &buff*) || !dlst_insert(chks, &elem, buff, 1))
			return (E_MLC);
		*chks = elem;
		(*chks)->n = idx;
		if (n_chrs - idx)
			if (!ft_substr((*rems)->str, idx, n_chrs - idx, &((*fd_e)->str)))
				return (E_MLC);
		else
		{
			printf("Nothing left !!\n");
			return (process_buff(read((*fd_e)->n, buff, BUFFER_SIZE), rems, fd_e, chks));
		}
	}
	return (nl_found);
}
*/


int	gnl_prep(size_t fd, t_dlst **fd_elem, t_dlst **chks, char **line)
{
	size_t	idx;
	size_t	nl_found;
	t_dlst	*elem;

	printf("Prep mon Prep with init fd_elem : %p\n", *fd_elem);
	while ((*fd_elem)->next && (*fd_elem)->next->n != fd)
		*fd_elem = (*fd_elem)->next;
	elem = *fd_elem;
	*fd_elem = (*fd_elem)->next;
	printf("fd_elem found ? %d\n", !!(*fd_elem));
	if (!(*fd_elem) || !dlst_insert(&elem, fd_elem, NULL, 2))
		return (0);
	(*fd_elem)->n = fd;
	if ((*fd_elem)->str)
	{
		printf("Prep : remaining str found\n");
		nl_found = scan_for_nl((*fd_elem)->str, &idx);
		if (nl_found)
			if (!(ft_substr((*fd_elem)->str, 0, ++idx, line)
			&& ft_substr((*fd_elem)->str, idx, SIZE_MAX, &((*fd_elem)->str))))
				return (0);
		else 
		{
			if (!dlst_insert(NULL, chks, (*fd_elem)->str, 0))
				return (0);
			(*fd_elem)->str = NULL;	
		}
	}
	return (1);
}
/*	
	if (*fd_elem)
	{
		*fd_elem = (*fd_elem)->next;
		if ((*fd_elem)->str)
		{
			*n_chrs = ft_substr((*fd_elem)->str, 0, SIZE_MAX, &((*rems)->str));
			malloc_free_p(0, (void **)&((*fd_elem)->str));
		}
	}
*/

char	*rec_liner(t_dlst **rems, t_dlst **fd_e, t_dlst **chks, size_t last_rd)
{
	size_t	n_chrs;
	size_t	idx;
	size_t	nl_found;
	t_dlst	*elem;

	n_chrs = read((*fd_e)->n, (*rems)->str, BUFFER_SIZE);
	if (!n_chrs || n_chrs == E_IFD)
		return (manage_eof(rems, fd_e, chks, NULL));
	else if (last_rd)
	{
		if (!ft_substr((*rems)->str, 0, n_chrs, &((*fd_e)->str)))
			return ((char *)E_MLC);//(manage_eof(rems, fd_e, chks, (char *)E_MLC));
		return (gather_line(chks));
	}
	nl_found = scan_for_nl((*rems)->str, &idx);
	idx += nl_found;
	if (!(dlst_insert(chks, &elem, NULL, 1)
		&& ft_substr((*rems)->str, 0, idx, &(elem->str))
		&& ft_substr((*rems)->str, idx, n_chrs - idx, &((*fd_e)->str))))
		return ((char *)E_MLC);//(manage_eof(rems, fd_e, chks, (char *)E_MLC));
	if (nl_found && (idx == n_chrs))
		return (rec_liner(rems, fd_e, chks, 1));
	else if (nl_found)
		return (gather_line(chks));
	return (rec_liner(rems, fd_e, chks, 0));
}

char	*get_next_line(int fd)
{
	static t_dlst	*rems;
	t_dlst			*fd_elem;
	char			*line;
	t_dlst			*chks;
	size_t			n_chrs;

	chks = NULL;
	line = NULL;
	fd_elem = rems;
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	else if ((!rems && !dlst_insert(NULL, &rems, NULL, 0)
		&& !malloc_free_p(sizeof(char) * BUFFER_SIZE, (void **)&(rems->str)))
		|| !gnl_prep(fd, &fd_elem, &chks, &line))
		return (manage_eof(&rems, &fd_elem, &chks, (char *)E_MLC));
	if (line)
		return (line);
	line = rec_liner(&rems, &fd_elem, &chks, 0);
	if (line == (char *)E_MLC)
		return (manage_eof(&rems, &fd_elem, &chks, (char *)E_MLC));
	return (line);
}
/*
	while (!line)
	{
		n_chrs = read(fd, rems->str, BUFFER_SIZE);
		if (!n_chrs || n_chrs == E_IFD)
			return (manage_eof(&rems, &fd_elem, &chks, &line));
		if (!process_buff(&n_chrs, &rems, &fd_elem, &chks))
			return (clear_all(&rems, &fd_elem, &chks));

	}
	if (!(fd_elem->str))
	{
		n_chrs = read(fd, rems->str, BUFFER_SIZE);
		if (!n_chrs)
			manage_eof();
		else
			fd_elem->str = ft_substr(rems->str, 0, n_chrs);
	}
	return (line);

	while (1)
	{
		n_chrs = process_buff(n_chrs, &rems, &fd_elem, &chks);
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
*/

