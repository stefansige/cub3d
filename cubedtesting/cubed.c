#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF


typedef struct s_vars {
    void *mlx;
    void *win;
}              t_vars;

typedef struct s_point {
    int x;
    int y;
}              t_point;

typedef struct s_player {
    t_point pos;
    t_point dir;
}              t_player;

typedef struct s_ray {
    t_point pos;
    t_point dir;
    t_point ldir;
    t_point rdir;
}              t_ray;

int worldMap[7][7]=
{
  {1,1,1,3,1,1,1},
  {1,0,0,0,0,0,1},
  {1,0,0,0,0,0,1},
  {1,0,0,0,0,0,1},
  {1,0,0,0,0,0,1},
  {1,0,0,0,0,0,1},
  {1,1,1,2,1,1,1}
};

void draw_line(int x0, int y0, int x1, int y1, t_vars *vars, int color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    while (1)
    {
        mlx_pixel_put(vars->mlx, vars->win, x0, y0, color);
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

void fillsquare(int x0, int y0, int x1, int y1, t_vars *vars, int color)
{
    for (int i = x0; i < x1; i++)
    {
        for (int j = y0; j < y1; j++)
        {
            mlx_pixel_put(vars->mlx, vars->win, i, j, color);
        }
    }
}

int main(void)
{
    t_vars vars;
    t_player player;

    player.pos.x = 350;
    player.pos.y = 350;

    player.dir.x = 50;
    player.dir.y = 0;

    t_ray ray;

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 1000, 1000, "cuba");
    draw_line(player.pos.x - 50, player.pos.y, player.pos.x + 50 , player.pos.y, &vars, GREEN);
    draw_line(player.pos.x, player.pos.y - 50, player.pos.x, player.pos.y + 50, &vars, GREEN);
    draw_line(100, 0, 100, 1000, &vars, BLUE);
    draw_line(200, 0, 200, 1000, &vars, BLUE);
    draw_line(300, 0, 300, 1000, &vars, BLUE);
    draw_line(400, 0, 400, 1000, &vars, BLUE);
    draw_line(500, 0, 500, 1000, &vars, BLUE);
    draw_line(600, 0, 600, 1000, &vars, BLUE);
    draw_line(700, 0, 700, 1000, &vars, BLUE);
    draw_line(800, 0, 800, 1000, &vars, BLUE);
    draw_line(900, 0, 900, 1000, &vars, BLUE);
    draw_line(0, 100, 1000, 100, &vars, BLUE);
    draw_line(0, 200, 1000, 200, &vars, BLUE);
    draw_line(0, 300, 1000, 300, &vars, BLUE);
    draw_line(0, 400, 1000, 400, &vars, BLUE);
    draw_line(0, 500, 1000, 500, &vars, BLUE);
    draw_line(0, 600, 1000, 600, &vars, BLUE);
    draw_line(0, 700, 1000, 700, &vars, BLUE);
    draw_line(0, 800, 1000, 800, &vars, BLUE);
    draw_line(0, 900, 1000, 900, &vars, BLUE);
    fillsquare(0, 0, 100, 1000, &vars, BLUE);
    fillsquare(900, 0, 1000, 1000, &vars, BLUE);
    fillsquare(0, 0, 1000, 100, &vars, BLUE);
    fillsquare(0, 900, 1000, 1000, &vars, BLUE);

    while (1)
    {

        mlx_loop(vars.mlx);
    }
    return (0);
}
