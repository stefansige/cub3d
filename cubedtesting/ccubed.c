#include <mlx.h>
#include <math.h> // Include math.h for abs function
#include <stdlib.h>
#include <stdio.h>
#define RED 0x00FF0000

int main()
{
    void *mlx;
    void *win;
    void *img;
    int i;
    int y;
    int width;
    int height;
    int line_length;
    int bpp;
    char *data;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 1000, 1000, "Hello, world!");
    img = mlx_xpm_file_to_image(mlx, "./textures/north.xpm", &i, &y);
    data = mlx_get_data_addr(img, &bpp, &line_length, &i);
    if (img == NULL)
    {
        printf("Error\n");
        exit(1);
    }
    for (int v = 0; v < 1024; v++)
    {
        for (int h = 0; h < 1024; h++)
        {
            mlx_pixel_put(mlx, win, h, v, *(unsigned int *)(data + (v * line_length) + (h * (bpp / 8))));
        }
    }
    mlx_loop(mlx);
}
