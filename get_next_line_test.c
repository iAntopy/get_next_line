/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 17:04:29 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/03 16:20:22 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

char	*get_next_line(int fd);

const char	file1[] = "demo_files/ex1.txt";
const char	file2[] = "demo_files/ex2.txt";
const char	file3[] = "demo_files/nlx3.txt";

static int	callsCounter[100];

void	printnl(char *str)
{
	if (!str)
		printf("(null)\n");
	else
	{
		while (*str)
		{
			if (*str == '\n')
				write(1, "$", 2);
			write(1, str++, 1);
		}
		write(1, ">", 1);
	}
}

void	testCall(int fd)
{
	char	*line;

	printf("\n\n<|--------------fd %d LINE %d-------------|>", fd - 2, ++callsCounter[fd]);
	fclose(stdout);
	line = get_next_line(fd);
	fopen(stdout);
	printf("\n<|--------------fd %d LINE %d-------------|>\n", fd - 2, callsCounter[fd]);
	printnl(line);
	free(line);
}

int	main(void)
{
	int	fd1;
	int	fd2;
	int	fd3;

	fd1 = open(file1, O_RDONLY);
	fd2 = open(file2, O_RDONLY);
	fd3 = open(file3, O_RDONLY);
	if (errno)
	{
		printf("ERROR : %s\n", strerror(errno));
		return (errno);
	}
	printf("TEST START !\n");

	testCall(fd1);
	testCall(fd2);
	testCall(fd1);
	testCall(fd2);
	testCall(fd1);
	testCall(fd2);
	testCall(fd1);
	testCall(fd2);
	testCall(fd1);
	testCall(fd2);
	testCall(fd3);
	testCall(fd1);
	testCall(fd3);
	testCall(fd3);
	testCall(fd3);
	testCall(fd3);

	printf("TEST OVER ! \n");
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}
