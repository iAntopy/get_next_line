/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 17:27:01 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/03 19:07:17 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*(str++))
		len++;
	return (len);
}

int	init_read_buffer(char *rems[])
{
	char	*rd_buff;

	if (rems[RD_IDX])
		return (1);
	rd_buff = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	if (!rd_buff)
		return (0);
	rems[RD_IDX] = rd_buff;
	return (1);
}

int	manage_eof(char **rems, int fd)
{
	int	i;

	if (rems[fd] && rems[fd] != (char *)SG_EOF)
		free(rems[fd]);
	rems[fd] = (char *)SG_EOF;
	if (!rems[RD_IDX])
		return (SG_EOF);
	i = -1;
	while (++i < MAX_FDS)
		if (rems[i] && rems[i] != (char *)SG_EOF)
			return (SG_EOF);
	free(rems[RD_IDX]);
	rems[RD_IDX] = NULL;
	return (SG_EOF);
}

int	ft_strndup(char *str, size_t n, char **ret_ptr)
{
	char	*dup;
	char	*d;

	if (!n)
	{
		*ret_ptr = NULL;
		return (1);
	}
	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (0);
	d = dup;
	while (n--)
		*(d++) = *(str++);
	*d = '\0';
	*ret_ptr = dup;
	return (1);
}

int	strjoin_swap(char **s1, char *s2, size_t len1, size_t len2)
{
	char	*line;
	char	*sp;
	char	*l;

	line = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!line)
		return (0);
	l = line;
	sp = *s1;
	while (len1--)
		*(l++) = *(sp++);
	while (len2--)
		*(l++) = *(s2++);
	*l = '\0';
	free(*s1);
	*s1 = line;
	return (1);
}
