/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 17:04:29 by iamongeo          #+#    #+#             */
/*   Updated: 2022/04/29 20:02:21 by iamongeo         ###   ########.fr       */
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
const char	file3[] = "demo_files/ex3.txt";

void	printnl(char *str)
{
	while (*str)
	{
		if (*str == '\n')
			write(1, "$", 2);
		write(1, str++, 1);
	}
}

int	main(void)
{
	int	fd1;
	int	fd2;
	int	fd3;

	char	*line;   

	fd1 = open(file1, O_RDONLY);
	fd2 = open(file2, O_RDONLY);
	fd3 = open(file3, O_RDONLY);
	if (errno)
	{
		printf("ERROR : %s\n", strerror(errno));
		return (errno);
	}
	printf("TEST START !\n");
	line = get_next_line(fd1);
	printf("\n\n<|--------------fd 1 LINE 1-------------|>\n");
//	printnl(line);
	printf("%s", line);
	free(line);
/*
	line = get_next_line(fd2);
	printf("\n\n<|--------------fd 2 LINE 1-------------|>\n");
	printnl(line);
	free(line);

	line = get_next_line(fd1);
	printf("\n\n<|--------------fd 1 LINE 2-------------|>\n");
	printnl(line);
	free(line);

	line = get_next_line(fd2);
	printf("\n\n<|--------------fd 2 LINE 2-------------|>\n");
	printnl(line);
	free(line);

	line = get_next_line(fd3);
	printf("\n\n<|--------------fd 3 LINE 1-------------|>\n");
	printnl(line);
	free(line);

	line = get_next_line(fd1);
	printf("\n\n<|--------------fd 1 LINE 3-------------|>\n");
	printnl(line);
	free(line);

	line = get_next_line(fd2);
	printf("\n\n<|--------------fd 2 LINE 3-------------|>\n");
	printnl(line);
	free(line);

	line = get_next_line(fd3);
	printf("\n\n<|--------------fd 3 LINE 2-------------|>\n");
	printnl(line);
	free(line);

	line = get_next_line(fd1);
	printf("\n\n<|--------------fd 1 LINE 4-------------|>\n");
	printnl(line);
	if (line)
		free(line);
	
	line = get_next_line(fd2);
	printf("\n\n<|--------------fd 2 LINE 4-------------|>\n");
	printnl(line);
	if (line)
		free(line);

	line = get_next_line(fd3);
	printf("\n\n<|--------------fd 3 LINE 3-------------|>\n");
	printnl(line);
	free(line);

	line = get_next_line(fd1);
	printf("\n\n<|--------------fd 1 LINE 5-------------|>\n");
	printnl(line);
	if (line)
		free(line);

	line = get_next_line(fd1);
	printf("\n\n<|--------------fd 2 LINE 5-------------|>\n");
	if (line)
		free(line);
*/
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}
