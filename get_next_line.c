/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:14:12 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/28 18:34:57 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

#include <stdio.h>

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

//	printf("Managing eof\n");
	if (clr == E_MLC)
	{
		join_clear_list(NULL, rems);
		join_clear_list(NULL, chks);
		*fd_e = NULL;
		return (NULL);
	}
//	printf("eof : fd str ptr %p\n", (*fd_e)->str);
//	printf("eof : first chunk %p\n", *chks);
	line = NULL;
	if (chks && *chks)
	{
//		printf("eof : first chk str : %s\n", (*chks)->str);
		line = gather_line(chks);
	}
//	printf("eof : gathered line : %s\n", line);
	if (!(*rems)->next || !(*rems)->next->next)
		join_clear_list(NULL, rems);
	else
	{
		(*fd_e)->prev->next = (*fd_e)->next;
		if ((*fd_e)->next)
			(*fd_e)->next->prev = (*fd_e)->prev;
		malloc_free_p(0, (void **)&((*fd_e)->str));
		malloc_free_p(0, (void **)fd_e);
//		printf("eof : frees OK\n");
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

//	printf("rec : start, is last ? %s, read from file desc %zu, buff ptr %p\n", last ?"TRUE":"FALSE", (*fd_e)->n, (*rems)->str);
	n_chrs = read((*fd_e)->n, (*rems)->str, BUFFER_SIZE);
//	printf("rec : n_chrs read %zu\n", n_chrs);
//	if (n_chrs == 0)
//		free((*rems)->str);
//	printf("rec : read buff ptr %p\n", (*rems)->str);
//	printf("rec : n_chrs == E_IFD ? %d\n", n_chrs == E_IFD);
	if (!n_chrs || n_chrs == E_IFD)
		return (manage_eof(rems, fd_e, chks, 0));
	nl_found = scan_for_nl((*rems)->str, n_chrs, &idx);
	idx += nl_found;
//	printf("rec : nl found (%s) idx %zu\n", nl_found?"TRUE":"FALSE", idx);
	rm = n_chrs - idx;
//	printf("rec : rm %zu\n", rm);
	elem = *chks;
	if ((last && !ft_substr((*rems)->str, 0, n_chrs, &((*fd_e)->str)))
		|| (!last && !dlst_insert(chks, &elem, NULL, idx))
		|| (!last && !ft_substr((*rems)->str, 0, idx, &(elem->str)))
		|| (!last && !ft_substr((*rems)->str, idx, rm, &((*fd_e)->str))))
		return (manage_eof(rems, fd_e, chks, E_MLC));
	*chks = elem;
//	printf("rec : pushed chunk len %zu\n", elem->n);
	if (last || (nl_found && rm))
		return (gather_line(chks));
	return (rec_liner(rems, fd_e, chks, nl_found));
}

static char	*gnl_prep(t_dlst **rems, t_dlst **fd_e, t_dlst **chks, size_t fd)
{
	size_t	idx;
	size_t	nl_found;
	char	*line;

//	printf("prep : Start\n");
	while ((*fd_e)->next && (*fd_e)->next->n != fd)
		*fd_e = (*fd_e)->next;
	*fd_e = (*fd_e)->next;
//	printf("prep : fd found ? %s\n", (*fd_e)?"TRUE":"FALSE");
	if (!(*fd_e) && !dlst_insert(rems, fd_e, NULL, SIZE_MAX))
		return (manage_eof(rems, fd_e, chks, E_MLC));
//	printf("prep : fd str found %s\n", (*fd_e)->str?"TRUE":"FALSE");
//	if ((*fd_e)->str)
//		printf("prep : recovered str : %s\n", (*fd_e)->str);
	(*fd_e)->n = fd;
//	printf("prep : fd ptr %p, fd str ptr %p, fd->n %zu\n",*fd_e, (*fd_e)->str, (*fd_e)->n);
	nl_found = scan_for_nl((*fd_e)->str, SIZE_MAX, &idx);
	idx += nl_found;
//	printf("prep : nl found ? %s, idx %zu, reached end ? %s\n", nl_found?"TRUE":"FALSE", idx, ((*fd_e)->str && !(*fd_e)->str[idx])?"TRUE":"FALSE");
//	if ((*fd_e)->str)
//		printf("prep : char at idx %c\n", (*fd_e)->str[idx]);
	if (!(*fd_e)->str || !nl_found || (nl_found && (*fd_e)->str[idx] == '\0'))
	{
//		printf("prep : sending whole fd str to recursion\n");
		if ((*fd_e)->str && !dlst_insert(NULL, chks, (*fd_e)->str, 0))
			return (manage_eof(rems, fd_e, chks, E_MLC));
		if (*chks)
			(*chks)->n = idx;
		(*fd_e)->str = NULL;
		return (rec_liner(rems, fd_e, chks, nl_found));
	}
//	printf("prep : splitting recoverd str, fd str ptrptr %p\n", &((*fd_e)->str));
	if (!ft_substr((*fd_e)->str, 0, idx, &line)
		|| !ft_substr((*fd_e)->str, idx, SIZE_MAX, &((*fd_e)->str)))
		return (manage_eof(rems, fd_e, chks, E_MLC));
//	printf("prep : exiting with line : %s\n", line);
	return (line);
}
/*
int	is_fd_in_list(int fd, t_dlst *rems)
{
	if (!rems)
	{
		printf("is fd : rems was cleared or not malloced\n");
		return (0);
	}
	printf("is fd : looking for fd\n");
	if (rems->n == fd)
		return (1);
	while (rems->next)
	{
		printf("is fd : rems %p, rems->next == rems ? %s\n", rems->next, (rems->next == rems)?"TRUE":"FALSE");
		if (rems->next->n == fd)
			return (1);
		rems = rems->next;
	}
	return (0);
}
*/

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
//	printf("gnl : rd buff already exists ? %s\n", rems?"TRUE":"FALSE");
	if ((!rems && !dlst_insert(NULL, &rems, NULL, 0))
		|| (!rems->str && !malloc_free_p(sizeof(char) * BUFFER_SIZE,
				(void **)&(rems->str))))
		return (manage_eof(&rems, &fd_elem, &chks, E_MLC));
//	printf("gnl : fd received : %d\n", fd);
//	printf("gnl : malloced rems %p, rems str %p\n", rems, rems->str);
	fd_elem = rems;
//	printf("gnl : post rems init rems : %p\n", rems);
	line = gnl_prep(&rems, &fd_elem, &chks, fd);
//	printf("gnl : prep over with line : %s\n", line);
	if (line == (char *)E_MLC)
		return (manage_eof(&rems, &fd_elem, &chks, E_MLC));
//	printf("gnl : fd_elem is still in list : %s\n", is_fd_in_list(fd, rems)?"TRUE":"FALSE");
	return (line);
}
