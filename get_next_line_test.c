/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 17:04:29 by iamongeo          #+#    #+#             */
/*   Updated: 2022/04/18 21:12:40 by iamongeo         ###   ########.fr       */
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

int	main(void)
{
	int	fd1;
//	int	fd2;
//	int	fd3;

	char	*line;   

	fd1 = open(file1, O_RDONLY);
//	fd2 = open(file2, O_RDONLY);
//	fd3 = open(file3, O_RDONLY);
	if (errno)
	{
		printf("ERROR : %s\n", strerror(errno));
		return (errno);
	}

	line = get_next_line(fd1);
	printf("\n\n<|--------------LINE 1-------------|>\n");
	printf("next line 1 : %s\n", line);
	free(line);
	line = get_next_line(fd1);
	printf("\n\n<|--------------LINE 2-------------|>\n");
	printf("next line 2: %s\n", line);
	free(line);
	line = get_next_line(fd1);
	printf("\n\n<|--------------LINE 3-------------|>\n");
	printf("next line 3: %s\n", line);
	free(line);

	close(fd1);
//	close(fd2);
//	close(fd3);
	return (0);
}
