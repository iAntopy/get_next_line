/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:14:12 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/05 19:07:31 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line2.h"

#include <stdio.h>

// This function returns 1 if get_next_line should conclude, join the chunks 
// and return joined line. If malloc error occures, returns -1, If should
// continue reading from file returns 0.

/*
t_list	*index_fd_list(t_list **fd_elem, int idx)
{
	t_list	*elem;
	int		i;

	if (!*(fd_elem) && !malloc_ptr(sizeof(t_list), (void **)fd_elem))
		return (NULL);
	elem = *fd_elem;
	i = -1;
	while (++i < idx)
	{
		if (!elem->next)
			if (!malloc_ptr(sizeof(t_list), (void **)&elem->next))
				return (NULL);
		elem = elem->next;
		elem->str = NULL;
	}
	return (elem);
}
*/
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

int	process_buff(t_dlst **chks, const char *buff, size_t n_chrs, char **rem)
{
	size_t	idx;

	printf("process_buff start inside. \n");
	if (buff)
	{
		idx = -1;
		while (++idx < n_chrs)
			if (buff[idx] == '\n')
				break ;
		printf("final idx : %zu\n", idx);
		if (buff[idx++] == '\n')
		{
			if (!dlst_push_new_substr(chks, buff, 0, idx))
				return (-1);
			if (buff[idx] != '\0')
			{
				if (!ft_substr(buff, idx, n_chrs - idx, rem))
					return (-1);
				printf("substr result : %s\n", *rem);
			}
			ft_print_chunks(*chks);
			return (1);
		}
		else
			if (!dlst_push_new_substr(chks, buff, 0, n_chrs))
				return (-1);
	}
	return (0);
}

int	gnl_prep(char **rems, int fd, size_t *n_chrs)
{
	char	*r;
	char	*f;

	printf("pre malloc, rems[RD_IDX] == NULL : %d\n", rems[RD_IDX] == NULL);
	if (!rems[RD_IDX] && !malloc_ptr(sizeof(char) * BUFFER_SIZE, (void **)&rems[RD_IDX]))
		return (0);
	printf("post malloc\n");
	*n_chrs = 0;
	r = rems[RD_IDX];
	if (rems[fd])
	{
		printf("fd rem found\n");
		f = rems[fd];
		while (*(f++))
			(*n_chrs)++;
		f = rems[fd];
		printf("Copying rem to read buffer.\n");
		while (*f)
			*(r++) = *(f++);
		printf("Copy result : %s\n", rems[RD_IDX]);
		free(rems[fd]);
		rems[fd] = NULL;
	}
	else
	{
		printf("No fd rem found\n ");
		*n_chrs = 1;
		rems[RD_IDX][0] = '\0';
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*rems[MAX_FDS + 2];
	char		*line;
	t_dlst		*chks;
	size_t		n_chrs;
	int			status;

	printf("Open rems[RD_IDX] == NULL : %d\n", rems[RD_IDX] == NULL);
	if (fd < 0 || MAX_FDS <= fd)
		return (NULL);
	chks = NULL;
	line = NULL;
	printf("Preping gnl \n");
	if (!gnl_prep(rems, fd, &n_chrs))
		return (NULL);
	printf("Prep success. read buff first char : ");
	if (rems[RD_IDX][0] == '\0')
		printf(">\n");
	else
		printf("%c\n", rems[RD_IDX][0]);
	while (n_chrs)
	{
		if (rems[RD_IDX][0])
		{
			printf("Processing buffer.\n");
			status = process_buff(&chks, rems[RD_IDX], n_chrs, &rems[fd]);
			printf("Processing finished with status : %d\n", status);
			if (status == -1 || (status == 1 && !gather_line(chks, &line)))
				return (NULL);
			else if (status == 1)
			{
				printf("returning line : %s\n", line);
				return (line);
			}
		}
		n_chrs = read(fd, rems[RD_IDX], BUFFER_SIZE);
		printf("Read buffer, n_chrs : %zd\n", n_chrs);
		if (n_chrs == -1 || (!n_chrs && !gather_line(chks, &line)))
			return (NULL);
		else if (line)
			return (line);
	}
	return (line);
}
