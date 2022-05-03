/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nl_counter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 18:26:07 by iamongeo          #+#    #+#             */
/*   Updated: 2022/04/28 18:43:02 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static const size_t	BUFFER_SIZE = 4096;

size_t	nl_counter(char	rd_buff[BUFFER_SIZE], size_t n)
{
	char	*r;
	size_t	count;

	r = rd_buff;
	while (n--)
		if (*(r++) == '\n')
			count++;
	return (count);
}

int main(int argc, char *argv[])
{
	int		fd;
	size_t	n_chrs;
	char	rd_buff[BUFFER_SIZE];

	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			return (1);

		n_chrs = read(fd, rd_buff, BUFFER_SIZE);
		printf("nl count : %zu", nl_counter(rd_buff, n_chrs));
		close(fd);
	}
	return (0);
}
