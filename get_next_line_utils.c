/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 17:27:01 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/02 17:58:29 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char *str)
{
	size_t len;

	len = 0;
	while (*(str++))
		len++;
	return (len);
}

int	manage_eof(char **rems, int fd)
{
	int	i;

	if (rems[fd])
		free(rems[fd]);
	i = 0;
	while (i < MAX_FDS)
		if (rems[i])
			return (0);
	free(rems[RD_IDX]);
	return (0);
}

int	ft_strndup(char *s1, ssize_t n, char **rem)
{
	char	*dup;
	char	*d;

	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (0);
	d = dup;
	while (n--)
		*(s1++) = *(d++);
	*rem = dup;
	return (1);
}

int	strjoin_swap(char **s1, char *s2, size_t len1, size_t len2)
{
	char	*line;
	char	*l;

	if (!(*s1))
	{
		*s1 = s2;
		return (1);
	}
	line = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!line)
		return (0);
	l = line;
	while (len1--)
		*(l++) = *(s1++);
	while (len2--)
		*(l++) = *(s2++);
	*l = '\0';
	free(*s1);
	*s1 = line;
	return (1);
}
