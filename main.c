#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include "libft.h"
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	char	*line;
	int		fd;

	if (argc != 2)
		return (0);
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		return (1);
	while (get_next_line(fd, &line) == 1)
	{
		printf("%s\n", line);
	}
}
