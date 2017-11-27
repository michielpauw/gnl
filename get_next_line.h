/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/14 08:40:27 by mpauw             #+#    #+#             */
/*   Updated: 2017/11/20 14:00:55 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define	GET_NEXT_LINE_H
# define	BUFF_SIZE 4

# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft/libft.h"

int	get_next_line(const int fd, char **line);

typedef struct		s_fd_list
{
	int					fd;
	int					read_r;
	int					eof;
	char				*buff;
	struct s_fd_list	*next;
}					t_fd_list;

#endif
