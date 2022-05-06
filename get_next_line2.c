/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:14:12 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/05 22:54:23 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line2.h"

#include <stdio.h>

void	ft_print_chunks(t_dlst *chks)
{
	int	i;

	i = 0;
	printf("[ ");
	if (!chks)
	{
		printf("\n]\n");
		return ;
	}
	printf("\n\t%d : %s", i++, chks->str);
	while (chks->next)
	{
		chks = chks->next;
		printf("\n\t%d : %s", i++, chks->str);
	}
	printf("\n]\n\n");
}

void	*manage_eof(size_t n_chrs, t_dlst *rems, t_dlst *fd_elem)
{
	if (n_chrs != 0)
		return (NULL);
	if (fd_elem->next)
	{
		fd_elem->prev->next = fd_elem->next;
		fd_elem->next->prev = fd_elem->prev;
	}
	else
		fd_elem->prev->next = NULL;
	free(fd_elem->str);
	free(fd_elem);
	if (!(rems->next))
	{
		free(rems->str);
		free(rems);
	}
	return (NULL);
}

// This function returns 1 if get_next_line should conclude, join the chunks 
// and return joined line. If malloc error occures, returns -1, If should
// continue reading from file returns 0.
int	process_buff(t_dlst **chks, const char *buff, size_t n_chrs, char **rem)
{
	size_t	idx;

	if (buff != NULL && n_chrs != 0)
	{
		idx = -1;
		while (++idx < n_chrs)
			if (buff[idx] == '\n')
				break ;
		if (buff[idx++] == '\n')
		{
			if (!dlst_push_substr(chks, buff, 0, idx))
				return (-1);
			if (buff[idx] != '\0')
				if (!ft_substr(buff, idx, n_chrs - idx, rem))
					return (-1);
			ft_print_chunks(*chks);
			return (1);
		}
		else
			if (!dlst_push_substr(chks, buff, 0, n_chrs))
				return (-1);
	}
	return (0);
}

int	gnl_prep(t_dlst **rems, t_dlst **fd_elem, int fd, size_t *n_chrs)
{
	*n_chrs = 0;
	if (!(*rems) && !dlst_push_substr(rems, NULL, 0, BUFFER_SIZE))
		return (0);
	*fd_elem = *rems;
	while ((*fd_elem)->next && (*fd_elem)->next->n != fd)
		*fd_elem = (*fd_elem)->next;
	if ((*fd_elem)->next)
	{
		*fd_elem = (*fd_elem)->next;
		while ((*fd_elem)->str[*n_chrs])
			(*rems)->str[*n_chrs] = (*fd_elem)->str[(*n_chrs)++];
		free((*fd_elem)->str);
		(*fd_elem)->str = NULL;
	}
	else
	{
		if (!dlst_push_substr(&((*fd_elem)->next), NULL, -1, -1))
			return (0);
		*fd_elem = (*fd_elem)->next;
		*n_chrs = 1;
		(*rems)->str[0] = '\0';
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

	if (fd < 0 || MAX_FDS <= fd || BUFFER_SIZE < 1)
		return (NULL);
	chks = NULL;
	line = NULL;
	if (!gnl_prep(&rems, &fd_elem, fd, &n_chrs))
		return (NULL);
	while (1)
	{
		n_chrs = process_buff(&chks, rems->str, n_chrs, &(fd_elem->str));
		if (n_chrs == -1 || (n_chrs == 1 && !gather_line(chks, &line)))
			return (NULL);
		else if (n_chrs == 1)
			return (line);
		n_chrs = read(fd, rems->str, BUFFER_SIZE);
		if (n_chrs == -1 || (!n_chrs && !gather_line(chks, &line)))
			return (manage_eof(n_chrs, rems, fd_elem));
		else if (line)
			return (line);
	}
}
