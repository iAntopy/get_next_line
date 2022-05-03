/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 17:26:05 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/03 19:06:05 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	scan_nl(char *buff, size_t len, size_t *ret_idx)
{
	*ret_idx = 0;
	while (len--)
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
		return (SG_CONTINUE);
	rems[fd] = NULL;
	len = ft_strlen(temp_rem);
	if (scan_nl(temp_rem, len, &idx))
	{
		*cur_len = idx;
		len = len - idx;
		if (!ft_strndup(temp_rem, idx, line)
			|| !ft_strndup(temp_rem + idx, len, &rems[fd]))
			return (E_MALLOC_ERR);
		free(temp_rem);
		return (SG_RETURN);
	}
	else
	{
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
		n_chrs = read(fd, rems[RD_IDX], BUFFER_SIZE);
		if (n_chrs <= 0)
			return (manage_eof(rems, fd));
		nl_found = scan_nl(rems[RD_IDX], n_chrs, &idx);
		if (!strjoin_swap(line, rems[RD_IDX], *cur_len, idx))
			return (E_MALLOC_ERR);
		*cur_len += idx;
		if (nl_found)
		{
			if (!ft_strndup(&rems[RD_IDX][idx], n_chrs - idx, &rems[fd]))
				return (E_MALLOC_ERR);
			return (SG_RETURN);
		}
	}
}

char	*get_next_line(int fd)
{
	static char	*rems[MAX_FDS + 1];
	char		*line;
	ssize_t		cur_len;
	ssize_t		status;

	line = NULL;
	cur_len = 0;
	if (fd < 0 || MAX_FDS <= fd || BUFFER_SIZE <= 0
		|| rems[fd] == (char *)SG_EOF)
		return (NULL);
	if (!rems[RD_IDX] && !init_read_buffer(rems))
		return (NULL);
	status = scan_rem(rems, fd, &line, &cur_len);
	if (status == SG_RETURN)
		return (line);
	else if (status == E_MALLOC_ERR)
		return (NULL);
	status = scan_fd(rems, fd, &line, &cur_len);
	if (line && status != E_MALLOC_ERR)
		return (line);
	return (NULL);
}
