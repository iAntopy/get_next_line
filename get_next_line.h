/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_new_line.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 21:29:08 by iamongeo          #+#    #+#             */
/*   Updated: 2022/04/18 21:55:51 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif

# define MAX_FDS 1028
# define INIT_USBL_CHKS 512

# define TRUE 1
# define FALSE 0

#include <stdlib.h>
#include <unistd.h>

typedef struct s_gnldata
{
	int		is_init;
	char	**chunks;
	int		max_chks;
	int		used_chks;
	char	*last_chks[MAX_FDS];
	size_t	last_nchrs[MAX_FDS];
	char	*rd_buff;
}	t_gnldata;

char	*get_next_line(int fd);
int		init_gnldata(t_gnldata *gnld);
int		realloc_double_chunks(t_gnldata *gnld);
int		clear_used_chunks(t_gnldata *gnld);
char	*ft_strndup(char *str, size_t n);
char	*ft_strjoin(char **strs, size_t size);

enum	err_codes
{
	E_INIT_GNLD = 0xff00,
	E_REALLOC_CHUNKS = 0xff01
};

enum	signal_codes
{
	SG_NO_NEWLINE = -0xfa00,
	SG_EOF = -0xfa01
};

#endif
