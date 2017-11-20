/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/16 13:39:05 by mpauw             #+#    #+#             */
/*   Updated: 2017/11/17 16:32:04 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	**get_buff(int fd, int *new_buff)
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
			return (&(next->buff));
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
	return (&(next->buff));
}

static void	reallocate_mem(char **str, int *prev_mem)
{
	char	*temp;
	int		i;

	i = 0;
	temp = *str;
	if (!(*str = (char *)malloc(sizeof(char) * (*prev_mem * 2))))
		return ;
	while (*(temp + i))
	{
		*(*str + i) = *(temp + i);
		i++;
	}
	*prev_mem *= 2;
	free(temp);
}

static int	fill_string(char **line, char **buff, int read_r, int *mem_line)
{
	int		len_line;
	int		i;
	int		end;

	len_line = ft_strlen(*line);
	i = -1;
	end = 0;
	while (++i < BUFF_SIZE)
	{
		ft_putnbr(i);
		ft_putchar(' ');
		if ((*buff)[i] == '\n')
		{
			(*buff)[i++] = 0;
			return (1);
		}
		if (len_line + 1 > *mem_line)
			reallocate_mem(line, mem_line);
		if ((*buff)[i])
		{
			*(*line + len_line++) = (*buff)[i];
			*(*line + len_line) = 0;
			(*buff)[i] = 0;
		}
		end = !(*buff)[i] ? 1 : 0;
	}
	ft_putnbr(end);
	return (end == 1 ? 3 : 2);
}

int			get_next_line(const int fd, char **line)
{
	int		mem_line;
	int		read_r;
	int		wrt;
	int		new_buff;
	char	**buff;

	wrt = 0;
	if (!line || !(*line = (char *)malloc(sizeof(char) * mem_line)))
		return (-1);
	**line = 0;
	new_buff = 0;
	buff = get_buff(fd, &new_buff);
	while (buff && (wrt == 0 || wrt == 2))
	{
		if (new_buff || wrt == 2)
		{
			if (0 >= ((read_r = read(fd, *buff, BUFF_SIZE)) && !(**line)))
				return (read_r);
			else if (!read_r)
				return (1);
		}
		wrt = fill_string(line, buff, read_r, &mem_line);
	}
	return ((wrt == 3) ? 0 : wrt);
}
