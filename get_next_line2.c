/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:14:12 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/06 19:47:15 by iamongeo         ###   ########.fr       */
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

char	*clear_all(t_dlst **rems, t_dlst **fd_elem, t_dlst **chks)
{
	printf("Clear All\n");
	printf("Clear All : pre clear :\n:: rems :\t%p\n:: fd_elem :\t%p\n:: chks :\t%p\n", *rems, *fd_elem, *chks);
	if (*rems)
		join_clear_list(NULL, rems, 0);
	if (*chks)
		join_clear_list(NULL, chks, 0);
	*fd_elem = NULL;
	printf("Clear All : post clear :\n:: rems :\t%p\n:: fd_elem :\t%p\n:: chks :\t%p\n", *rems, *fd_elem, *chks);
	return (NULL);
}

char	*manage_eof(size_t n_chrs, t_dlst **rems, t_dlst **fd_e, t_dlst **chks)
{
	t_dlst	*elem;

	printf("Managing EOF\n");
	elem = *fd_e;
	if (n_chrs == E_MLC || !(elem->prev->prev))
		return (clear_all(rems, fd_e, chks));
	elem->prev->next = elem->next;
	if (elem->next)
		elem->next->prev = elem->prev;
	free(elem->str);
	elem = elem->prev;
	free(*fd_e);
	*fd_e = NULL;
	return (NULL);
}

// This function returns 1 if get_next_line should conclude, join the chunks 
// and return joined line. If malloc error occures, returns -1, If should
// continue reading from file returns 0.
size_t	process_buff(t_dlst **chks, const char *buff, size_t n, char **rem)
{
	size_t	idx;
	char	*s;
	t_dlst	*elem;

	printf("Processing buffer, n : %zu\n", n);
	if (buff && n)
	{
		idx = -1;
		while (++idx < n)
			if (buff[idx] == '\n')
				break ;
		if (buff[idx++] == '\n')
		{
			printf("Proc buff : \\n found at idx %zu\n", idx);
			if (!ft_substr(buff, 0, idx, &s) || !dlst_insert(chks, &elem, s, 1))
				return (E_MLC);
			printf("Proc Buff : new chunk malloced with str : %s\n", (*chks)->str);
			if (buff[idx] != '\0')
			{
				if (!ft_substr(buff, idx, n - idx, rem))
					return (E_MLC);
				printf("Proc Buff : rest of str malloced to rem : %s\n", *rem);
			}
			return (0);
		}
		else
		{
			if (!ft_substr(buff, 0, n, &s) || !dlst_insert(chks, &elem, s, 1))
				return (E_MLC);
			printf("Proc Buff : No \\n found so malloc whole line to chks : %s\n", (*chks)->str);
		}
	}
	return (1);
}

int	gnl_prep(t_dlst **rems, t_dlst **fd_elem, size_t fd, size_t *n_chrs)
{
	printf("gnl prep :\n");
	if (!(*rems) && !dlst_insert(NULL, rems, malloc(sizeof(char) * BUFFER_SIZE), 0))
		return (0);
	printf("gnl prep : rems at ptr %p\n", *rems);
	printf("gnl prep : dlst insert SUCCESS\n");	
	*fd_elem = *rems;
	printf("gnl prep : rems prev, rems next: %p, %p\n", (*rems)->prev, (*rems)->next);
	while ((*fd_elem)->next && (*fd_elem)->next->n != fd)
	{
		printf("gnl prep : fd_elem prev, fd_elem next : %p, %p\n", (*fd_elem)->prev,(*fd_elem)->next);
		*fd_elem = (*fd_elem)->next;
	}
	printf("gnl prep : fd no fd SUCCESS\n");
	if ((*fd_elem)->next)
	{
		printf("gnl prep : fd dlst found!\n");
		*fd_elem = (*fd_elem)->next;
		while ((*fd_elem)->str[*n_chrs])
			(*rems)->str[*n_chrs] = (*fd_elem)->str[(*n_chrs)++];
		free((*fd_elem)->str);
		(*fd_elem)->str = NULL;
	}
	else
	{
		printf("gnl prep : fd dlst NOT found!\n");
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
	n_chrs = 0;
	if (!gnl_prep(&rems, &fd_elem, fd, &n_chrs))
		return (clear_all(&rems, &fd_elem, &chks));
	while (1)
	{
		printf("wild\n");
		n_chrs = process_buff(&chks, rems->str, n_chrs, &(fd_elem->str));
		if (n_chrs == E_MLC || (!n_chrs && !gather_line(&chks, &line, &n_chrs)))
			return (clear_all(&rems, &fd_elem, &chks));//(manage_eof(n_chrs, &rems, &chks));
		else if (!n_chrs && line)
			return (line);
		n_chrs = read(fd, rems->str, BUFFER_SIZE);
		if (n_chrs == E_IFD || (!n_chrs && !gather_line(&chks, &line, &n_chrs)))
			return (manage_eof(n_chrs, &rems, &fd_elem, &chks));
		else if (!n_chrs && line)
			return (line);
	}
}