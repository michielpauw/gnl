/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/16 13:39:05 by mpauw             #+#    #+#             */
/*   Updated: 2017/11/20 13:20:03 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_fd_list	*get_buff(int fd, int *new_buff)
{
	static t_fd_list	*list = 0;
	t_fd_list			*next;
	t_fd_list			*prev;
	char				buff[BUFF_SIZE + 1];

	next = list;
	prev = NULL;
	while (next)
	{
		if (next->fd == fd)
			return (next);
		prev = next;
		next = next->next;
	}
	ft_memset(buff, 0, BUFF_SIZE + 1);
	if (!(next = (t_fd_list *)malloc(sizeof(t_fd_list *))))
		return (NULL);
	if (prev)
		prev->next = next;
	else
		list = next;
	next->fd = fd;
	next->buff = buff;
	next->next = NULL;
	*new_buff = 1;
	return (next);
}

static void	reallocate_mem(char **str, int *prev_mem)
{
	char	*temp;
	int		i;
	char	*temp2;

	i = 0;
	temp = *str;
	ft_putstr("before the return");
	temp2 = (char *)malloc(sizeof(char) * 20);
	ft_putstr("does it do this?");
	if (!(*str = (char *)malloc(sizeof(char) * (*prev_mem * 2)))) {
		ft_putstr("returning");
		return ;
	}
	ft_putstr("after the return");
	while (*(temp + i))
	{
		*(*str + i) = *(temp + i);
		i++;
	}
	*prev_mem *= 2;
	free(temp);
}

static int		fill_string(char **line, t_fd_list *buff, int *mem_line)
{
	int		len_line;
	int		i;

	len_line = ft_strlen(*line);
	i = -1;
	while (++i < BUFF_SIZE && i < buff->read_r)
	{
		if (len_line + 1 >= *mem_line)
		{
			reallocate_mem(line, mem_line);	
		}
		if ((buff->buff)[i] == '\n')
		{
			(buff->buff)[i++] = 0;
			return (1);
		}
		if ((buff->buff)[i])
		{
			*(*line + len_line++) = (buff->buff)[i];
			*(*line + len_line) = 0;
			(buff->buff)[i] = 0;
		}
	}
	return (buff->eof = ((i == buff->read_r && buff->read_r < BUFF_SIZE) ? 1 : 2));
}

int				get_next_line(const int fd, char **line)
{
	int			mem_line;
	int			wrt;
	int			new_buff;
	t_fd_list	*buff;

	wrt = 0;
	mem_line = 1;
	if (!line || !(*line = (char *)malloc(sizeof(char) * mem_line)))
		return (-1);
	**line = 0;
	new_buff = 0;
	buff = get_buff(fd, &new_buff);
	if (buff->eof == 1)
		return (0);
	while (buff && (wrt == 0 || wrt == 2))
	{
		if (new_buff || wrt == 2)
		{
			if (0 >= ((buff->read_r = read(fd, buff->buff, BUFF_SIZE))) && !(**line))
				return (buff->read_r);
			else if (!buff->read_r)
				return (1);
		}
		wrt = fill_string(line, buff, &mem_line);
	}
	return (wrt);
}
