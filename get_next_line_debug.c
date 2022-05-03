/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 17:26:05 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/03 16:28:51 by iamongeo         ###   ########.fr       */
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

static int	scan_rem(char **rems, int fd, char **line, ssize_t *cur_len)
{
	size_t	idx;
	char	*temp_rem;
	size_t	len;

	temp_rem = rems[fd];
	if (!temp_rem)
	{
		printf("no rem to recover from rems[fd]\n");
		return (SG_CONTINUE);
	}
	printf("recovered rem from rems[fd] : %s\n", temp_rem);
	rems[fd] = NULL;
	len = ft_strlen(temp_rem);
	if (scan_nl(temp_rem, &idx))
	{
		printf("nl found in rem at idx %zu\n", idx);
		*cur_len = idx;
		len = len - idx;
		if (!ft_strndup(temp_rem, idx, line) || !ft_strndup(temp_rem + idx, len, &rems[fd]))
			return (E_MALLOC_ERR);
		free(temp_rem);
		return (SG_RETURN);
	}
	else
	{
		printf("no nl found in rem. rem becomes line.\n");
		*line = temp_rem;
		*cur_len = len;
		return (SG_CONTINUE);
	}
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
			printf("EOF ALERT !\n");
			return (manage_eof(rems, fd));
		}
		nl_found = scan_nl(rems[RD_IDX], &idx);
		printf("nl found ? %s, idx found : %zu\n", (nl_found) ? "TRUE":"FALSE", idx);
		if (!strjoin_swap(line, rems[RD_IDX], *cur_len, idx))
		{
			printf("ERROR : str_join failed!\n");
			return (E_MALLOC_ERR);
		}
		printf("joined line : %s\n", *line);
		*cur_len += idx;
		if (nl_found)
		{
			printf("nl found at idx %zu, cur_len %zd\n", idx, *cur_len);
			if (!ft_strndup(&rems[RD_IDX][idx], n_chrs - idx, &rems[fd]))
			{
				printf("ERROR : strdup failed!");
				return (E_MALLOC_ERR);
			}
			return (SG_RETURN);
		}
		else
			rems[fd] = NULL;
	}
}

char	*get_next_line(int fd)
{
	static char	*rems[MAX_FDS + 1];
	char		*line;
	ssize_t		cur_len;
	ssize_t		status;

	printf("\n\nNew get_next_line call on fd %d\n", fd);
	line = NULL;
	cur_len = 0;
	printf("rems[fd] : %p vs (char *)SG_EOF : %p\n", rems[fd], (char *)SG_EOF);
	printf("rems[RD_IDX] ptr : %p\n", rems[RD_IDX]);
	if (rems[fd] == (char *)SG_EOF)
		printf("rems[fd] ptr == SG_EOF\n");
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
	status = scan_rem(rems, fd, &line, &cur_len);
//	if (rems[fd] && scan_rem(&rems[fd], &line, &cur_len) == SG_RETURN)
	if (status == SG_RETURN)
		return (line);
	else if (status == E_MALLOC_ERR)
		return (NULL);
//	printf("is rems[fd] NULL ? %s\n", (!rems[fd]) ? "TRUE":"FALSE");
//	if (!rems[fd] || !scan_fd(rems, fd, &line, &cur_len))
	status = scan_fd(rems, fd, &line, &cur_len);
	if (!rems[RD_IDX])
		printf("\n[][]~[]~[]~~[]~~~[]~READ~BUFFER~CLEARED~[]~~~[]~~[]~[]~[][]\n");

	if (status == E_MALLOC_ERR || status == SG_EOF)
	{
		if (status == SG_EOF)
			printf("fd %d reached EOF\n", fd);
		else if (status == E_MALLOC_ERR)
			printf("scan_rem failed with malloc error.\n");
		return (NULL);
	}
	printf("EXIT fd %d\n", fd);
	return (line);
}
