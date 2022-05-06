/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:10:58 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/05 22:49:13 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	GET_NEXT_LINE2_H
# define GET_NEXT_LINE2_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif

# define MAX_FDS 65535
# define RD_IDX MAX_FDS
# define FT_SIZE_MAX 0ULL - 1

# include <stdlib.h>
# include <unistd.h>

typedef struct s_dlst
{
	struct s_dlst	*prev;
	struct s_dlst	*next;
	char			*str;
	size_t			n;
}	t_dlst;

int	malloc_ptr(size_t n, void **ret_ptr);
int	ft_substr(const char *str, size_t start, size_t n, char **ret);
int	dlst_push_substr(t_dlst **dlst, const char *str, size_t start, size_t len);
int	gather_line(t_dlst *dlst, char **ret_line);

#endif
