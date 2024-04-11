#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF

typedef struct s_point {
    int x;
    int y;
}              t_point;

typedef struct s_player {
    t_point pos;
    int dir;
}              t_player;

typedef struct s_vars {
    void *mlx;
    void *win;
    t_player player;
}              t_vars;

typedef struct s_ray {
    t_point pos;
    int dir;
}              t_ray;

int worldMap[10][10]=
{
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1}
};

int ft_fixangle(int angle)
{
    angle = (int)angle % 360;
    if (angle < 0)
        angle += 360;
    return (angle);
}

double torad(int angle)
{
    return (angle * M_PI / 180);
}

int ft_abs(int n)
{
    if (n < 0)
        return (-n);
    return (n);
}

void draw_line(int x0, int y0, int x1, int y1, t_vars *vars, int color)
{
    int dx = ft_abs(x1 - x0);
    int dy = ft_abs(y1 - y0);
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

void ft_movepoint(int *x, int *y, int angle_degrees, int distance) {
    // Ensure the angle is within the range of 0 to 360 degrees
    angle_degrees %= 360;
    if (angle_degrees < 0)
        angle_degrees += 360;

    // Convert angle from degrees to radians

    double angle_radians = angle_degrees * M_PI / 180.0;

    // Calculate new floating-point coordinates
    double new_x = *x + distance * (cos(angle_radians) * 5);
    double new_y = *y + distance * (sin(angle_radians) * 5);

    // Round and update the integer coordinates
    *x = (int)round(new_x);
    *y = (int)round(new_y);
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

int ft_key(int keycode, t_vars *vars)
{
    if (keycode == 0xff1b)
        exit(0);
    if (keycode == 0xff53)
        vars->player.dir += 10;
    if (keycode == 0xff51)
        vars->player.dir -= 10;
    if (keycode == 0xff52)
    {
        ft_movepoint(&vars->player.pos.x, &vars->player.pos.y, vars->player.dir, 10);
        printf("x: %d, y: %d\n", vars->player.pos.x, vars->player.pos.y);
    }
    if (keycode == 0xff54)
    {
        ft_movepoint(&vars->player.pos.x, &vars->player.pos.y, vars->player.dir + 180, 10);
        printf("x: %d, y: %d\n", vars->player.pos.x, vars->player.pos.y);
    }
    return (0);
}

int ft_rays(t_vars *vars)
{
    t_ray ray;

    for (int i = 0; i < 100; i++) // Iterate over 100 rays
        {
            // Calculate the ray direction based on the player's direction and field of view
            int dir = vars->player.dir - 50 + i;

            // Ensure the angle is within the range of 0 to 360 degrees
            dir = ft_fixangle(dir);
            ray.dir = dir;
            ray.pos.x = vars->player.pos.x;
            ray.pos.y = vars->player.pos.y;
            int rx;
            int ry;
            double hipx;
            double hipy;
            if (ray.dir < 90)
            {
                rx = 100 - ray.pos.x % 100;
                ry = 100 - ray.pos.y % 100;
                    hipx = rx / cos(torad(ray.dir));
                    hipy = ry / sin(torad(ray.dir));
                    if (hipx <= hipy)
                    {
                        ray.pos.x += rx;
                        ray.pos.y += hipx * sin(torad(ray.dir));
                    }
                    else
                    {
                        ray.pos.y += ry;
                        ray.pos.x += hipy * cos(torad(ray.dir));
                    }
            }
            else if (ray.dir < 180)
            {
                rx = ray.pos.x % 100;
                ry = 100 - ray.pos.y % 100;
                    hipy = rx / cos(torad(ray.dir - 90));
                    hipx = ry / sin(torad(ray.dir - 90));
                    if (hipx < hipy)
                    {
                        ray.pos.x -= rx;
                        ray.pos.y += hipx * cos(torad(ray.dir - 90));
                    }
                    else
                    {
                        ray.pos.y += ry;
                        ray.pos.x -= hipy * sin(torad(ray.dir - 90));
                    }
            }
            else if (ray.dir < 270)
            {
                rx = ray.pos.x % 100;
                ry = ray.pos.y % 100;
                    hipx = rx / cos(torad(ray.dir - 180));
                    hipy = ry / sin(torad(ray.dir - 180));
                    if (hipx <= hipy)
                    {
                        ray.pos.x -= rx;
                        ray.pos.y -= hipx * sin(torad(ray.dir - 180));
                    }
                    else
                    {
                        ray.pos.y -= ry;
                        ray.pos.x -= hipy * cos(torad(ray.dir - 180));
                    }
            }
            else
            {
                rx = 100 - ray.pos.x % 100;
                ry = ray.pos.y % 100;
                    hipy = rx / cos(torad(ray.dir - 270));
                    hipx = ry / sin(torad(ray.dir - 270));
                    if (hipx <= hipy)
                    {
                        ray.pos.x += rx;
                        ray.pos.y -= hipx * cos(torad(ray.dir - 270));
                    }
                    else
                    {
                        ray.pos.y -= ry;
                        ray.pos.x += hipy * sin(torad(ray.dir - 270));
                    }
            }
            draw_line(vars->player.pos.x, vars->player.pos.y, ray.pos.x, ray.pos.y, vars, RED);
        }

    return (0);
}

int main(void)
{
    t_vars vars;

    vars.player.pos.x = 350;
    vars.player.pos.y = 350;
    vars.player.dir = 90;

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 1000, 1000, "cuba");
/*   draw_line(vars.player.pos.x - 50, vars.player.pos.y, vars.player.pos.x + 50 , vars.player.pos.y, &vars, GREEN);
    draw_line(vars.player.pos.x, vars.player.pos.y - 50, vars.player.pos.x, vars.player.pos.y + 50, &vars, GREEN);
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
    */
    mlx_hook(vars.win, 2, 1L << 0, ft_key, &vars);
    mlx_loop_hook(vars.mlx, ft_rays, &vars);
    mlx_loop(vars.mlx);
    return (0);
}
