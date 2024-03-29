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