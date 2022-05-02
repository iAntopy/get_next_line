/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 17:26:05 by iamongeo          #+#    #+#             */
/*   Updated: 2022/04/27 23:51:47 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


#include <stdio.h>


static ssize_t	scan_nl(char *buff)
{
	char	*b;

	b = buff - 1;
	while (*(++b))
		if (*b == '\n')
			return (b - buff);
	return (SG_NO_NEWLINE);
}

static size_t	scan_remainder(t_gnldata *gd, char **line, char *rmd)
{
	ssize_t	idx;
	int		fd;

	fd = gd->cur_fd;
	idx = scan_nl(rmd);
	if (idx == SG_NO_NEWLINE)
		*line = rmd;
	else
	{
		*line = ft_strndup(rmd, idx + 1);
		gd->remainders[fd] = ft_strndup(rmd + idx + 1, ft_strlen(rmd) - idx - 1);
		free(rmd);
	}
	return (idx);
}

static ssize_t	scan_fd(t_gnldata *gd, char **line, ssize_t *cur_len, ssize_t *n_chrs)
{
	ssize_t	idx;
	char	*temp_line;
	int		fd;

	fd = gd->cur_fd;
	while (*n_chrs)
	{
		idx = scan_nl(gd->rd_buff);
		if (idx == SG_NO_NEWLINE)
		{
			temp_line = ft_strjoin(*line, gd->rd_buff, *cur_len, *n_chrs);
			if (!temp_line)
				return (E_MALLOC_ERR);
			*cur_len += *n_chrs;
			free(*line);
			*line = temp_line;
			gd->remainders[fd] = NULL;
			*n_chrs = read(fd, gd->rd_buff, BUFFER_SIZE);
		}
		else
		{
			temp_line = ft_strjoin(*line, gd->rd_buff, *cur_len, idx + 1);
			if (!temp_line)
				return (E_MALLOC_ERR);
			*cur_len += idx + 1;
			free(*line);
			*line = temp_line;
			gd->remainders[fd] = ft_strndup(gd->rd_buff + idx + 1, *n_chrs - idx - 1);
			return (SG_LINE_COMPLET);
		}
	}
	gd->remainders[fd] = (char *)SG_EOF;
	return (SG_EOF);
}

void	print_nl_count(char	*str)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] == '\n')
			count++;

	printf("nb nl : %d\n", count);

}

char	*get_next_line(int fd)
{
	static t_gnldata	gd;
	char				*line;
	ssize_t				n_chrs;
	ssize_t				cur_len;

	line = NULL;
	gd.cur_fd = fd;
	cur_len = 0;
	if (gd.remainders[fd] == (char *)SG_EOF)
		return (NULL);
	else if (gd.remainders[fd])
	{
//		printf("gnl : scanning remainder %s\n", gd.remainders[fd]);
		n_chrs = scan_remainder(&gd, &line, gd.remainders[fd]);
//		printf("gnl : scanning remainder PASSED, line : %s\n", line);
		if (n_chrs != SG_NO_NEWLINE)
			return (line);
		else
//			printf("gnl : post remainder scan NO_NEWLINE\n");
		cur_len = ft_strlen(line);
//		printf("gnl : cur_len : %zd\n", cur_len);
	}
//	printf("gnl : remainder scan PASSED\n");
	n_chrs = read(fd, gd.rd_buff, BUFFER_SIZE);
	if (!n_chrs)
		return (NULL);
	if (!line)
		line = ft_strndup("", 0);
//	printf("gnl : read PASSED\n");
	n_chrs = scan_fd(&gd, &line, &cur_len, &n_chrs);
//	printf("gnl : scan_fd PASSED \n");
//	print_nl_count(gd.rd_buff);
	if (n_chrs == E_MALLOC_ERR)
		return (NULL);
	return (line);
}
