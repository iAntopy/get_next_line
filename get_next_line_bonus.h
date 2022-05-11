/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:10:58 by iamongeo          #+#    #+#             */
/*   Updated: 2022/05/11 15:26:23 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
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
int		malloc_ptr(size_t n, void **ret_ptr);
int		ft_substr(const char *str, size_t start, size_t n, char **ret);
int		dlst_insert(t_dlst **dlst, t_dlst **elem, char *mstr, int push_app);
int		join_clear_list(char *line, t_dlst **elem, int do_join);
int		gather_line(t_dlst **chks, char **ret_line, size_t *n_chrs);

enum	e_fail_codes
{
	E_MLC = 0ULL - 2,
	E_IFD = 0ULL - 1
};

#endif
