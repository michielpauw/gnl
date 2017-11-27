/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_mine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 18:12:40 by mpauw             #+#    #+#             */
/*   Updated: 2017/11/20 15:20:26 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		file;
	int		amt_read;
	char	*line;
	char	buff[32];

	if (argc != 2)
		return (0);
	file = open(argv[1], O_RDONLY);
	if (file < 0)
		return (0);

	while ((amt_read = get_next_line(file, &line)))
	{
		ft_putstr(line);
		ft_putchar('\n');
		free(line);
	}
}
