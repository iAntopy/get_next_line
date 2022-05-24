/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:10:58 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/23 20:22:52 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif
# ifndef SIZE_MAX
#  define SIZE_MAX 0xffffffffffffffff
# endif

# include <stdlib.h>
# include <unistd.h>

typedef struct s_dlst
{
	struct s_dlst	*prev;
	struct s_dlst	*next;
	char			*str;
	size_t			n;
}	t_dlst;

char	*get_next_line(int fd);
int		malloc_free_p(size_t n, void **ret_ptr);
int		ft_substr(char *str, size_t n, char **ret, size_t *n_chrs);
//int		ft_substr(char *str, size_t start, size_t n, char **ret);
int		dlst_insert(t_dlst **dlst, t_dlst **elem, char *mstr, int push_app);
int		join_clear_list(char *line, t_dlst **elem);
//int		gather_line(t_dlst **chks, char **ret_line, size_t *n_chrs);
char	*gather_line(t_dlst **chks);

enum	e_fail_codes
{
	E_EOF = SIZE_MAX - 2,
	E_MLC = SIZE_MAX - 1,
	E_IFD = SIZE_MAX
};

#endif
