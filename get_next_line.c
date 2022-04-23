/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:55:14 by iamongeo          #+#    #+#             */
/*   Updated: 2022/04/20 13:57:07 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
/*
static void	ft_strcpy(char *dst, char *src, size_t n_chrs)
{
	while (n_chrs--)
		*(dst++) = *(src++);
}
*/
static ssize_t	scan_nl(char *rd_buff, ssize_t n_chrs)
{
	ssize_t	i;

	i = -1;
	while (rd_buff[++i] && (i < n_chrs))
		if (rd_buff[i] == '\n')
			return (i + 1);
	return (SG_NO_NEWLINE);
}

static int	mng_chnk_brk_check(t_gnldata *gd, int fd, ssize_t n_chrs, size_t idx)
{
	if (idx == SG_NO_NEWLINE)
	{
		gd->chunks[gd->used_chks++] = ft_strndup(gd->rd_buff, n_chrs);
		if (gd->chunks[gd->used_chks - 1])
			printf("chunk manager : ft_strndup SUCCESS\n");
		else
			printf("chunk manager : ft_strndup FAILURE\n");
		if (n_chrs != BUFFER_SIZE)
		{
			gd->last_nchrs[fd] = (size_t)SG_EOF;
			return (1);
		}
		return (0);
	}
	else
	{
		printf("\'\\n\' found at index : %zu\n", idx);
		gd->chunks[gd->used_chks++] = ft_strndup(gd->rd_buff, idx);
		if (gd->chunks[gd->used_chks - 1])
			printf("chunk manager : ft_strndup SUCCESS\n");
		else
			printf("chunk manager : ft_strndup FAILURE\n");
		gd->last_nchrs[fd] = n_chrs - idx;
		if (gd->last_nchrs[fd])
			gd->last_chks[fd] = ft_strndup(gd->rd_buff + idx, n_chrs - idx);
		return (1);
	}
}

static void	scan_fd(t_gnldata *gd, int fd, ssize_t n_chrs)
{
	size_t	idx;

	while (n_chrs)
	{
		if (gd->used_chks >= gd->max_chks)
			realloc_double_chunks(gd);
		idx = scan_nl(gd->rd_buff, n_chrs);
		if (mng_chnk_brk_check(gd, fd, n_chrs, idx))
			break ;
		else
			n_chrs = read(fd, gd->rd_buff, BUFFER_SIZE);
	}
}

char	*get_next_line(int fd)
{
	static t_gnldata	gd;
	char				*line;

	line = NULL;
	if (!gd.is_init)
		if (init_gnldata(&gd) == E_INIT_GNLD)
			return (NULL);
	printf("gdnl initialized : SUCCESS\n");
	if (gd.last_nchrs[fd] == (size_t)SG_EOF)
		return (NULL);

	gd.used_chks = 0;
	printf("fd last nchrs not EOF :  SUCCESS\n");
	if (gd.last_chks[fd])
	{
		gd.chunks[gd.used_chks++] = gd.last_chks[fd];
		//ft_strcpy(gd.rd_buff, gd.last_chks[fd], gd.last_nchrs[fd]);
		//free(gd.last_chks[fd]);
		gd.last_chks[fd] = NULL;
		printf("fd last chunk recovered :  SUCCESS\n");
		printf("recovered chunk : %s\n\n", gd.chunks[0]);
		scan_fd(&gd, fd, gd.last_nchrs[fd]);
		printf("fd recovered chunks scanned and malloced :  SUCCESS\n");
	}
	else
	{
		scan_fd(&gd, fd, read(fd, gd.rd_buff, BUFFER_SIZE));
		printf("new fd\'s chunks scanned and malloced :  SUCCESS\n");
	}
	if (gd.used_chks)
	{
		if (gd.used_chks == 1)
			line = gd.chunks[0];
		else
		{
			line = ft_strjoin(gd.chunks, gd.used_chks);
			clear_used_chunks(&gd);
			printf("chunks cleared :  SUCCESS\n");
		}
	}
	printf("line joined %d chunks :  SUCCESS\n", gd.used_chks);
	return (line);
}
