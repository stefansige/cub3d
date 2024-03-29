#include "cub3d.h"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr("Invalid number of arguments!\n");
		ft_putstr("Usage: ./cub3d [map]\n");
		return (1);
	}
	if (ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4) != 0)
	{
		ft_putstr("Invalid map file!\n");
		return (1);
	}
	
}