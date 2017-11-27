/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/16 13:39:05 by mpauw             #+#    #+#             */
/*   Updated: 2017/11/20 15:15:19 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_fd_list	*get_buff(int fd, int *new_buff)
{
	static t_fd_list	*list = NULL;
	t_fd_list			*current;
	t_fd_list			*prev;

	*new_buff = 0;
	current = list;
	prev = NULL;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		prev = current;
		current = current->next;
	}
	if (!(current = (t_fd_list *)malloc(sizeof(t_fd_list))) \
			|| !(current->buff = (char*)malloc(sizeof(char) * BUFF_SIZE)))
		return (NULL);
	if (prev)
		prev->next = current;
	else
		list = current;
	current->fd = fd;
	current->next = NULL;
	*new_buff = 1;
	return (current);
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

static int		fill_string(char **line, t_fd_list *current_fd, int *alloc_size)
{
	int		len_line;
	int		i;

	len_line = ft_strlen(*line);
	i = -1;
	while (++i < BUFF_SIZE && i < current_fd->read_r)
	{
		if (len_line + 1 >= *alloc_size)
		{
			reallocate_mem(line, alloc_size);
		}
		if ((current_fd->buff)[i] == '\n')
		{
			(current_fd->buff)[i] = 0;
			return (1);
		}
		if ((current_fd->buff)[i])
		{
			*(*line + len_line++) = (current_fd->buff)[i];
			*(*line + len_line) = 0;
			(current_fd->buff)[i] = 0;
		}
	}
	return (current_fd->eof = ((i == current_fd->read_r && current_fd->read_r < BUFF_SIZE) ? 1 : 2));
}

int				get_next_line(const int fd, char **line)
{
	int			alloc_size;
	int			fill_status;
	int			is_new_buff;
	t_fd_list	*current_fd;

	current_fd = get_buff(fd, &is_new_buff);
	alloc_size = BUFF_SIZE;
	if (!line || !(*line = (char *)malloc(sizeof(char) * alloc_size)) || !current_fd)
		return (-1);
	if (current_fd->eof == 1)
		return (0);
	**line = 0;
	fill_status = 0;
	while (current_fd && (fill_status == 0 || fill_status == 2))
	{
		if (is_new_buff || fill_status == 2)
		{
			is_new_buff = 0;
			if (0 >= ((current_fd->read_r = read(fd, current_fd->buff, BUFF_SIZE))) && !(**line))
				return (current_fd->read_r);
			else if (!current_fd->read_r)
				return (1);
		}
		fill_status = fill_string(line, current_fd, &alloc_size);
	}
	return (fill_status);
}
