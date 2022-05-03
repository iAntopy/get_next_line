/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_new_line.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 21:29:08 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/03 19:39:28 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif

# define MAX_FDS 65536 
# define RD_IDX MAX_FDS

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
int		init_read_buffer(char **rems);
int		manage_eof(char **rems, int fd);
size_t	ft_strlen(const char *str);
int		ft_strndup(char *str, size_t n, char **ret_ptr);
int		strjoin_swap(char **s1, char *s2, size_t len1, size_t len2);

enum	e_err_codes
{
	E_MALLOC_ERR = -0xff00LL,
	E_INVALID_FD = -0xff01LL
};

enum	e_signal_codes
{
	SG_RETURN = -0xfa01LL,
	SG_EOF = -0xfa02LL,
	SG_CONTINUE = - 0xfa03LL
};

#endif
