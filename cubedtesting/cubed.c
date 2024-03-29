#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct s_vars {
    void *mlx;
    void *win;
}              t_vars;

typedef struct s_point {
    int x;
    int y;
}              t_point;

void draw_line(int x0, int y0, int x1, int y1, t_vars *vars)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    while (1)
    {
		usleep(10000);
        mlx_pixel_put(vars->mlx, vars->win, x0, y0, 0xFFFFFF);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

int main(void)
{
    t_vars vars;
    t_point p0 = {100, 0};
    t_point p1 = {100, 600};

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 800, 600, "Bresenham's Line Algorithm");
    draw_line(p0.x, p0.y, p1.x, p1.y, &vars);
    mlx_loop(vars.mlx);
    return (0);
}
