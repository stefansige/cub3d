#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define WHITE 0xFFFFFF
#define YELLOW 0xFFFF00

typedef struct s_point {
    double x;
    double y;
}              t_point;

typedef struct s_player
{
    t_point pos;
    float dir;
}              t_player;

typedef struct s_image {
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
    int img_width;
    int img_height;
}              t_image;

typedef struct s_vars {
    void *mlx;
    void *win;
    t_image north;
    t_image south;
    t_image east;
    t_image west;
    t_player player;
}              t_vars;

typedef struct s_ray {
    t_point pos;
    float dir;
    double hip;
    double rx;
    double ry;
    float planeangle;
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

int ft_abs(int n)
{
    if (n < 0)
        return (-n);
    return (n);
}


float ft_fixangle(float angle)
{
    angle = fmod(angle, 360.0f);
    if (angle < 0.0f)
        angle += 360.0f;
    return angle;
}

float angleDifference(float playerangle, float rayangle) {
    float diff = fabsf(playerangle - rayangle);
    diff = fmodf(diff, 360.0f);

    if (diff > 180.0f) {
        diff = 360.0f - diff;
    }
    return diff;
}

double torad(double angle)
{
    return (angle * M_PI / 180.0);
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

void ft_movepoint(double *x, double *y, int angle_degrees, int distance) {
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
    {
        vars->player.dir += 10;
        vars->player.dir = ft_fixangle(vars->player.dir);
        printf("dir: %d\n", vars->player.dir);
    }
    if (keycode == 0xff51)
    {
        vars->player.dir -= 10;
        vars->player.dir = ft_fixangle(vars->player.dir);
        printf("dir: %d\n", vars->player.dir);
    }
    if (keycode == 0xff52)
    {
        ft_movepoint(&vars->player.pos.x, &vars->player.pos.y, vars->player.dir, 10);
        printf("x: %f, y: %f\n", vars->player.pos.x, vars->player.pos.y);
    }
    if (keycode == 0xff54)
    {
        ft_movepoint(&vars->player.pos.x, &vars->player.pos.y, vars->player.dir + 180, 10);
        printf("x: %f, y: %f\n", vars->player.pos.x, vars->player.pos.y);
    }
    mlx_clear_window(vars->mlx, vars->win);
    return (0);
}
/*
promeni da pos bude double mora zbog preciznosti sa coskovima
*/

void ft_display(t_vars *vars, double raydist, t_ray *ray, int raynum, int orientation, int imgx)
{
    int linelen = round(100000 / raydist);
    if (linelen > 800)
        linelen = 800;
    unsigned int y_start = 400 - (linelen / 2);
    unsigned int y_end = 400 + (linelen / 2);
    t_image *img;
    double xscale;
    double yscale; 
    int start = raynum * 10;
    int end = raynum * 10 + 10;
    int color = RED;

    if (orientation == 1)
    {
        img = &vars->north;
    }
    else if (orientation == 3)
    {
        img = &vars->south;
    }
    else if (orientation == 4)
    {
        img = &vars->west;
    }
    else if (orientation == 2)
    {
        img = &vars->east;
    }
    xscale = (double)img->img_width / (double)linelen;
    yscale = (double)img->img_height / (double)linelen;
    imgx = imgx * xscale * (linelen / 100);
    while (start < end)
    {
        for (int j = y_start; j < y_end; j++)
        {
            color = *(unsigned int *)(img->addr + (int)(j * img->line_length) + (int)(imgx  * (img->bits_per_pixel / 8)));
            mlx_pixel_put(vars->mlx, vars->win, start, j, color);
        }
        start++;
        imgx += xscale;
    }
}

int ft_rays(t_vars *vars)
{
    t_ray ray;

    double hipx;
    double hipy;
    int dir;
    int hit;
    double perpwalldist;
    int orientation;
    int imgx;

    for (int i = 0; i < 80; i++)
        {

            dir = vars->player.dir - 40 + i;


            dir = ft_fixangle(dir);
            ray.dir = dir;
            ray.pos.x = vars->player.pos.x;
            ray.pos.y = vars->player.pos.y;
            hit = 0;
            ray.hip = 0;
            while(hit == 0)
            {
                if (ray.dir < 90)
                {
                    ray.rx = 100.0 - fmod(ray.pos.x, 100.0);
                    ray.ry = 100.0 - fmod(ray.pos.y, 100.0);
                        hipx = ray.rx / fabs(cos(torad(ray.dir)));
                        hipy = ray.ry / fabs(sin(torad(ray.dir)));
                        if (hipx <= hipy)
                        {
                            ray.pos.x += ray.rx;
                            ray.pos.y += hipx * fabs(sin(torad(ray.dir)));
                            if (worldMap[((int)ray.pos.x - 1) / 100][(int)ray.pos.y / 100] == 1)
                            {
                                hit = 1;
                                orientation = 3;
                                imgx = ray.rx;
                            }
                            else if (worldMap[(int)ray.pos.x / 100][((int)ray.pos.y - 1) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 2;
                                imgx = 100 - ray.ry;
                            }
                        }
                        else
                        {
                            ray.pos.y += ray.ry;
                            ray.pos.x += hipy * fabs(cos(torad(ray.dir)));
                            if (worldMap[((int)ray.pos.x - 1) / 100][(int)ray.pos.y / 100] == 1)
                            {
                                hit = 1;
                                orientation = 3;
                                imgx = ray.rx;
                            }
                            else if (worldMap[(int)ray.pos.x / 100][((int)ray.pos.y - 1) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 2;
                                imgx = 100 - ray.ry;
                            }
                        }
                }
                else if (ray.dir < 180)
                {
                    ray.rx = fmod(ray.pos.x, 100.0);
                    ray.ry = 100.0 - fmod(ray.pos.y, 100.0);
                    if (ray.rx == 0)
                        ray.rx = 100;
                        hipx = ray.rx / fabs(cos(torad(180 - ray.dir)));
                        hipy = ray.ry / fabs(sin(torad(180 - ray.dir)));
                        if (hipx <= hipy)
                        {
                            ray.pos.x -= ray.rx;
                            ray.pos.y += hipx * fabs(sin(torad(180 - ray.dir)));
                            if (worldMap[((int)ray.pos.x - 1) / 100][(int)(ray.pos.y - 1) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 4;
                                imgx = ray.ry;
                            }
                            else if (worldMap[((int)ray.pos.x) / 100][(int)ray.pos.y / 100] == 1)
                            {
                                hit = 1;
                                orientation = 3;
                                imgx = 100 - ray.rx;
                            }
                        }
                        else
                        {
                            ray.pos.y += ray.ry;
                            ray.pos.x -= hipy * fabs(cos(torad(180 - ray.dir)));
                            if (worldMap[((int)ray.pos.x - 1) / 100][(int)(ray.pos.y - 1) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 4;
                                imgx = ray.ry;
                            }
                            else if (worldMap[((int)ray.pos.x) / 100][(int)ray.pos.y / 100] == 1)
                            {
                                hit = 1;
                                orientation = 3;
                                imgx = 100 - ray.rx;
                            }
                        }
                }
                else if (ray.dir < 270)
                {
                    ray.rx = fmod(ray.pos.x, 100.0);
                    ray.ry = fmod(ray.pos.y, 100.0);
                    if (ray.rx == 0)
                        ray.rx = 100;
                    if (ray.ry == 0)
                        ray.ry = 100;
                        hipx = ray.rx / fabs(cos(torad(ray.dir - 180)));
                        hipy = ray.ry / fabs(sin(torad(ray.dir - 180)));
                        if (hipx <= hipy)
                        {
                            ray.pos.x -= ray.rx;
                            ray.pos.y -= hipx * fabs(sin(torad(ray.dir - 180)));
                            if (worldMap[((int)ray.pos.x - 1) / 100][((int)ray.pos.y) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 4;
                                imgx = 100 - ray.ry;
                            }
                            else if (worldMap[((int)ray.pos.x) / 100][((int)ray.pos.y - 1) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 1;
                                imgx = ray.rx;
                            }
                        }
                        else
                        {
                            ray.pos.y -= ray.ry;
                            ray.pos.x -= hipy * fabs(cos(torad(ray.dir - 180)));
                            if (worldMap[((int)ray.pos.x - 1) / 100][((int)ray.pos.y) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 4;
                                imgx = 100 - ray.ry;
                            }
                            else if (worldMap[((int)ray.pos.x) / 100][((int)ray.pos.y - 1) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 1;
                                imgx = ray.rx;
                            }
                        }
                }
                else
                {
                    ray.rx = 100.0 - fmod(ray.pos.x, 100.0);
                    ray.ry = fmod(ray.pos.y, 100.0);
                    if (ray.ry == 0)
                        ray.ry = 100;
                        hipx = ray.rx / cos(torad(360 - ray.dir));
                        hipy = ray.ry / sin(torad(360 - ray.dir));
                        if (hipx <= hipy)
                        {
                            ray.pos.x += ray.rx;
                            ray.pos.y -= hipx * sin(torad(360 - ray.dir));
                            if (worldMap[(int)ray.pos.x / 100][((int)ray.pos.y) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 2;
                                imgx = ray.ry;
                            }
                            else if (worldMap[(int)(ray.pos.x - 1) / 100][((int)ray.pos.y - 1) / 100] == 1)
                            {
                                hit = 1; 
                                orientation = 1;
                                imgx = 100 - ray.rx;
                            }
                        }
                        else
                        {
                            ray.pos.y -= ray.ry;
                            ray.pos.x += hipy * cos(torad(360 - ray.dir));
                            if (worldMap[(int)ray.pos.x / 100][((int)ray.pos.y) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 2;
                                imgx = ray.ry;
                            }
                            else if (worldMap[(int)(ray.pos.x - 1) / 100][((int)ray.pos.y - 1) / 100] == 1)
                            {
                                hit = 1;
                                orientation = 1;
                                imgx = 100 - ray.rx;
                            }
                        }
                }
                if (hipx <= hipy)
                    ray.hip += hipx;
                else
                    ray.hip += hipy;
            }
            ray.planeangle = angleDifference(vars->player.dir, ray.dir);
            perpwalldist = ray.hip * cos(torad(ray.planeangle));
            ft_display(vars, perpwalldist, &ray, i, orientation, imgx);
        }
    return (0);
}

int main(void)
{
    t_vars vars;

    vars.player.pos.x = 500;
    vars.player.pos.y = 500;
    vars.player.dir = 90;

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 800, 800, "cuba");
    vars.north.img = mlx_xpm_file_to_image(vars.mlx, "./textures/north.xpm", &vars.north.img_width, &vars.north.img_height);
    vars.south.img = mlx_xpm_file_to_image(vars.mlx, "./textures/south.xpm", &vars.south.img_width, &vars.south.img_height);
    vars.east.img = mlx_xpm_file_to_image(vars.mlx, "./textures/east.xpm", &vars.east.img_width, &vars.east.img_height);
    vars.west.img = mlx_xpm_file_to_image(vars.mlx, "./textures/west.xpm", &vars.west.img_width, &vars.west.img_height);
    if (vars.north.img == NULL || vars.south.img == NULL || vars.east.img == NULL || vars.west.img == NULL)
    {
        printf("Invalid map\n");
        exit(0);
    }
    vars.north.addr = mlx_get_data_addr(vars.north.img, &vars.north.bits_per_pixel, &vars.north.line_length, &vars.north.endian);
    vars.south.addr = mlx_get_data_addr(vars.south.img, &vars.south.bits_per_pixel, &vars.south.line_length, &vars.south.endian);
    vars.east.addr = mlx_get_data_addr(vars.east.img, &vars.east.bits_per_pixel, &vars.east.line_length, &vars.east.endian);
    vars.west.addr = mlx_get_data_addr(vars.west.img, &vars.west.bits_per_pixel, &vars.west.line_length, &vars.west.endian);

    /*
    draw_line(vars.player.pos.x - 50, vars.player.pos.y, vars.player.pos.x + 50 , vars.player.pos.y, &vars, GREEN);
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
