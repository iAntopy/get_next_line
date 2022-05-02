/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 17:26:05 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/02 17:09:27 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


#include <stdio.h>


static int	scan_nl(char *buff, size_t *ret_idx)
{
	*ret_idx = 0;
	while (*buff)
	{
		(*ret_idx)++;
		if (*buff == '\n')
			return (1);
		buff++;
	}
	return (0);
}

static ssize_t	scan_rem(char **rem, char **line, ssize_t *cur_len)
{
	size_t	idx;
	char	*temp_rem;
	size_t	len;	

	temp_rem = *rem;
	*rem = NULL;
	if (!scan_nl(temp_rem, &idx))
	{
		*line = temp_rem;
		idx = SG_RETURN;
	}
	else
	{
		len = ft_strlen(temp_rem) - idx;
		if (!ft_strndup(temp_rem, idx, line) || !ft_strndup(temp_rem + idx, len, rem))
			return (E_MALLOC_ERR);
		free(temp_rem);
	}
	*cur_len = idx;
	return (idx);
}

static ssize_t	scan_fd(char **rems, int fd, char **line, ssize_t *cur_len)
{
	size_t	idx;
	ssize_t	n_chrs;
	int		nl_found;

	while (1)
	{
		printf("while loop pass!\n");
		n_chrs = read(fd, rems[RD_IDX], BUFFER_SIZE);
		if (!n_chrs)
		{
			printf("EOF ALLERT !\n");
			return (manage_eof(rems, fd));
		}
		nl_found = scan_nl(rems[RD_IDX], &idx);
		if (!strjoin_swap(line, rems[RD_IDX], *cur_len, idx))
		{
			printf("ERROR : str_join failed!\n");
			return (0);
//			return (E_MALLOC_ERR);
		}
		printf("joined line : %s\n", *line);
		*cur_len += idx;
		if (nl_found)
		{
			printf("nl found at idx %zu, cur_len %zd\n", idx, *cur_len);
			if (!ft_strndup(&rems[RD_IDX][idx], n_chrs - idx, &rems[fd]))
			{
				printf("ERROR : strdup failed!");
				return (0);
//				return (E_MALLOC_ERR);
			}
			return (1);
		}
		else
			rems[fd] = NULL;
	}
	printf("ERROR : unexpected while loop break");
}

char	*get_next_line(int fd)
{
	static char	*rems[MAX_FDS];
	char		*line;
	ssize_t		cur_len;

	line = NULL;
	cur_len = 0;
	if (fd < 0 || MAX_FDS <= fd || BUFFER_SIZE <= 0 || rems[fd] == (char *)SG_EOF)
	{
		printf("Returned NULL bc -- fd < 0 : %d, MAX_FDS <= fd : %d, BUFFER_SIZE <= 0 : %d, rems[fd] == SG_EOF : %d\n", fd < 0, MAX_FDS <= fd, BUFFER_SIZE <= 0, rems[fd] == (char *)SG_EOF);
		return (NULL);
	}
	if (!rems[RD_IDX] && !init_read_buffer(rems))
	{
		printf("Returned NULL bc -- !rems[RD_IDX] : %d, init read failed : %d\n",  !rems[RD_IDX], 0);
		return (NULL);
	}
	if (rems[fd] && scan_rem(&rems[fd], &line, &cur_len) == SG_RETURN)
	{
		printf("Returned NULL bc -- rems[fd] doesn't exists or scan rem failed. ");
		return (line);
	}
	if (!rems[fd] && !scan_fd(rems, fd, &line, &cur_len))
	{
		printf("Returned NULL bc -- no rems[fd] or scan_fd failed.");
		return (NULL);
	}
	printf("EXIT fd %d\n", fd);
	return (line);
}
