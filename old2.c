/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/14 08:22:48 by mpauw             #+#    #+#             */
/*   Updated: 2017/11/16 12:46:23 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_fd_list *new_buff(int fd)
{
	t_fd_list	*list;
	char		*buff;
	int			i;

	i = 0;
	if (!(list = (t_fd_list *)malloc(sizeof(t_fd_list *))))
		return (NULL);
	list->fd = fd;
	if (fd > 0)
	{
		if (!(buff = (char *)malloc(sizeof(char) * BUFF_SIZE)))
			return (NULL);
		while (i < BUFF_SIZE)
			buff[i++] = 0;
	}
	else
		buff = NULL;
	list->buff = buff;
	list->next = NULL;
	return list;
}

static char	**set_get_buff(int fd)
{
	static t_fd_list	*list;
	t_fd_list			*next;
	t_fd_list			*prev;

	if (!list)
		list = new_buff(0);
	next = list;
	while (next)
	{
		if (next->fd == fd)
			return (&(next->buff));
		prev = next;
		next = next->next;
	}
	next = new_buff(fd);
	prev->next = next;
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

static int	handle_buff(char *buff, char **line, int *buff_read, int *mem_line)
{
	int		size_line;
	int		i;

	size_line = ft_strlen(*line);
	i = 0;
	while (!(*(buff + *buff_read)) && *buff_read < BUFF_SIZE)
		(*buff_read)++;
	while (*buff_read < BUFF_SIZE && buff[*buff_read])
	{
		if (size_line + i + 1 >= *mem_line)
			reallocate_mem(line, mem_line);
		if (buff[*buff_read] == '\n')
		{
			buff[*buff_read] = 0;
			(*buff_read)++;
			return (1);
		}
		*(*line + size_line + i) = buff[*buff_read];
		*(*line + size_line + i++ + 1) = 0;
		buff[(*buff_read)++] = 0;
	}
	if (!buff[*buff_read] && *buff_read < BUFF_SIZE)
		return (1);
	return (0);
}

int			get_next_line(const int fd, char **line)
{
	int			buff_read;
	int			read_r;
	int			end;
	int			mem_line;
	char		**buff;

	end = 0;
	buff_read = 0;
	mem_line = 32;
	if (!line || !(*line = (char *)malloc(sizeof(char) * mem_line)))
		return (-1);
	**line = 0;
	buff = set_get_buff(fd);
	while (!(end = handle_buff(*buff, line, &buff_read, &mem_line)))
	{
		if (buff_read >= BUFF_SIZE)
		{
			ft_memset(*buff, 0, BUFF_SIZE);
			buff_read = 0;
			if ((read_r = read(fd, *buff, BUFF_SIZE)) <= 0 && !(**line))
				return (read_r);
			else if (read_r == 0)
				return (1);
		}
	}
	return (end);
}
