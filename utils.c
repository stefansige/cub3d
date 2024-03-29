#include "cub3d.h"

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

int ft_strncmp(char *s1, char *s2, int n)
{
	int i;

	i = 0;
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return (s1[i] - s2[i]);
}

void ft_putstr(char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		write(1, &str[i++], 1);
}

void		*ft_memcpy(void *s1, const void *s2, size_t n)
{
	size_t	i;
	char	*dest;
	char	*src;

	dest = (char*)s1;
	src = (char*)s2;
	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size);
	ft_memcpy(new_ptr, ptr, size);
	return (new_ptr);
}