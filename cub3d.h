#ifndef CUB3D_H
# define CUB3D_H

#include <unistd.h>
typedef struct s_cub_data {
    char **map;
} t_cub_data;

int		ft_strlen(char *str);
int		ft_strncmp(char *s1, char *s2, int n);
void	ft_putstr(char *str);

#endif