#include "libft.h"

void	ft_realloc(void **s, size_t old_size, size_t new_size)
{
	size_t			i;
	unsigned char	*m;

	m = *(unsigned char **)s;
	if (!(*s = (unsigned char *)malloc(sizeof(unsigned char) * new_size)))
		return ;
	i = 0;
	while (i < old_size)
	{
		(*(unsigned char **)s)[i] = m[i];
		i++;
	}
	free(m);
}

/*
void	ft_realloc(char *str, size_t *prev_mem)
{
	char	*temp;
	size_t	i;

	i = 0;
	temp = str;
	if (!(str = (char *)malloc(sizeof(char) * (*prev_mem * 2 + 1))))
		return ;
	while (*(temp + i))
	{
		*(str + i) = *(temp + i);
		i++;
	}
	*prev_mem *= 2;
	free(temp);
}
*/
