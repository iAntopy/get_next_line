/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:32:39 by iamongeo          #+#    #+#             */
/*   Updated: 2022/04/25 17:08:00 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	init_gnldata(t_gnldata *gnld)
{
	gnld->chunks = (char **)malloc(sizeof(char *) * INIT_USBL_CHKS);
//	gnld->rd_buff = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	if (!gnld->chunks)
		return (E_INIT_GNLD);
	gnld->max_chks = INIT_USBL_CHKS;
	gnld->used_chks = 0;
	gnld->is_init = TRUE;
	return (1);
}

int	realloc_double_chunks(t_gnldata *gnld)
{
	char	**original_chunks;
	int		i;

	i = -1;
	original_chunks = gnld->chunks;
	gnld->chunks = (char **)malloc(sizeof(char *) * gnld->max_chks * 2);
	if (!gnld->chunks)
		return (E_REALLOC_CHUNKS);
	while (++i < gnld->max_chks)
		gnld->chunks[i] = original_chunks[i];
	free(original_chunks);
	gnld->max_chks *= 2;
	return (1);
}

int	clear_used_chunks(t_gnldata *gnld)
{
	int	i;

	i = -1;
	while (++i < gnld->used_chks)
		free(gnld->chunks[i]);
	gnld->used_chks = 0;
	return (1);
}

char	*ft_strndup(char *str, size_t n)
{
	char	*sdup;
	char	*s;

	s = str;
	while (*s)
		s++;
	if (n > (size_t)(s - str))
		n = (size_t)(s - str);
	sdup = (char *)malloc(sizeof(char) * (n + 1));
	if (!sdup)
		return (NULL);
	s = sdup;
	while (n--)
		*(s++) = *(str++);
	*s = '\0';
	return (sdup);
}

#include <stdio.h>

char	*ft_strjoin(char **strs, size_t size)
{
	char	*line;
	char	*l;
	char	*s;
	size_t	len;

	len = 0;
	l = (char *)size;
	printf("ft_strjoin : l =  %p\n", l);
	while (--l)
	{
		printf("ft_strjoin : (int)l =  %d\n", (int)l);
		s = strs[(int)l];
		printf("ft_strjoin : strs vs strs + (int)l =  %p, %p\n", strs, strs + (int)l);
		printf("ft_strjoin : strs[(int)l] vs strs[1] vs s : %p, %p, %p\n", strs[(int)l], strs[1], s);

		while (*(s++))
			len++;
		printf("ft_strjoin : line counted\n");
	}
	printf("ft_strjoin : pre malloc\n");
	line = (char *)malloc(sizeof(char) * (len + 1));
	printf("ft_strjoin : post malloc\n");
	if (!line)
		return (NULL);
	printf("ft_strjoin : malloc SUCCESS\n");
	l = line;
	while (size--)
	{
		s = *(strs++);
		while (*s)
			*(l++) = *(s++);
	}
	printf("ft_strjoin : line copied SUCCESS\n");
	*l = '\0';
	return (line);
}
