#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define BUFFER_SIZE 4096


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
    t_image floor;
    t_image ceiling;
    char *buff;
    int **map;
    unsigned int ceilingcolor;
    unsigned int floorcolor;
    t_player player;
}              t_vars;

typedef struct s_ray {
    t_point pos;
    float dir;
    double hip;
    int orientation;
    double rx;
    double ry;
    double hipx;
    double hipy;
    double imgx;
    int hit;
    float planeangle;
    double perpwalldist;
}              t_ray;

typedef struct s_disvars
{
    int linelen;
    unsigned int y_start;
    unsigned int y_end;
    t_image *img;
    double yadd;
    int screenx;
    int color;
    double imgy;
}              t_disvars;

float ft_fixangle(float angle)
{
    angle = fmod(angle, 360.0f);
    if (angle < 0.0f)
        angle += 360.0f;
    return angle;
}

float angleDifference(float playerangle, float rayangle)
{
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
void ft_movepoint(double *x, double *y, float angle_degrees, int distance)
{
    double new_x;
    double new_y;
    ft_fixangle(angle_degrees);

    new_x = *x + distance * (cos(torad(angle_degrees)));
    new_y = *y + distance * (sin(torad(angle_degrees)));
    *x = (int)round(new_x);
    *y = (int)round(new_y);
}
int ft_checkcollision(t_vars *vars, double x, double y, float angle)
{
    double collision_x;
    double collision_y;
    ft_fixangle(angle);

    collision_x = x + 75 * (cos(torad(angle)));
    collision_y = y + 75 * (sin(torad(angle)));
    if (vars->map[(int)(collision_y / 100)][(int)(collision_x / 100)] != 2)
        return (0);
    return (1);
}

void ft_free(int **map)
{
    int i;

    i = 0;
    while (map[i])
    {
        free(map[i]);
        i++;
    }
    free(map);
}

void ft_exit(t_vars *vars, char *str)
{
    printf("%s", str);
    if (vars->north.img != NULL)
        mlx_destroy_image(vars->mlx, vars->north.img);
    if (vars->south.img != NULL)
        mlx_destroy_image(vars->mlx, vars->south.img);
    if (vars->east.img != NULL)
        mlx_destroy_image(vars->mlx, vars->east.img);
    if (vars->west.img != NULL)
        mlx_destroy_image(vars->mlx, vars->west.img);
    if (vars->floor.img != NULL)
        mlx_destroy_image(vars->mlx, vars->floor.img);
    if (vars->ceiling.img != NULL)
        mlx_destroy_image(vars->mlx, vars->ceiling.img);
    if (vars->map != NULL)
        ft_free(vars->map);
    if (vars->buff != NULL)
        free(vars->buff);
    if (vars->win != NULL)
        mlx_destroy_window(vars->mlx, vars->win);
    if (vars->mlx != NULL)
    {
        mlx_destroy_display(vars->mlx);
        free(vars->mlx);
    }
    exit(0);
}

int ft_key(int keycode, t_vars *vars)
{
    if (keycode == 0xff1b)
        ft_exit(vars, "Bye bye\n");
    if (keycode == 0xff53)
        vars->player.dir = ft_fixangle(vars->player.dir + 10);
    if (keycode == 0xff51)
        vars->player.dir = ft_fixangle(vars->player.dir - 10);
    if (keycode == 0x0077)
        if (ft_checkcollision(vars, vars->player.pos.x, vars->player.pos.y, vars->player.dir))
            ft_movepoint(&vars->player.pos.x, &vars->player.pos.y, vars->player.dir, 50);
    if (keycode == 0x0073)
        if (ft_checkcollision(vars, vars->player.pos.x, vars->player.pos.y, vars->player.dir - 180))
            ft_movepoint(&vars->player.pos.x, &vars->player.pos.y, vars->player.dir - 180, 50);
    if (keycode == 0x0061)
        if (ft_checkcollision(vars, vars->player.pos.x, vars->player.pos.y, vars->player.dir - 90))
            ft_movepoint(&vars->player.pos.x, &vars->player.pos.y, vars->player.dir - 90, 50);
    if (keycode == 0x0064)
        if (ft_checkcollision(vars, vars->player.pos.x, vars->player.pos.y, vars->player.dir + 90))
            ft_movepoint(&vars->player.pos.x, &vars->player.pos.y, vars->player.dir + 90, 50);
    printf("dir: %f\n", vars->player.dir);
    return (0);
}

t_image *ft_getimg(t_vars *vars, int orientation)
{
    if (orientation == 1)
        return (&vars->north);
    else if (orientation == 2)
        return (&vars->east);
    else if (orientation == 3)
        return (&vars->south);
    else if (orientation == 4)
        return (&vars->west);
    return (NULL);
}

void ft_display(t_vars *vars, t_ray *ray, float raynum)
{
    t_disvars d;

    ray->perpwalldist = ray->hip * cos(torad(ray->planeangle));
    d.linelen = round(50000 / ray->perpwalldist);
    if (d.linelen > HEIGHT)
        d.linelen = HEIGHT;
    d.y_start = 400 - (d.linelen / 2);
    d.y_end = 400 + (d.linelen / 2);
    d.imgy = 0.0;

    d.img = ft_getimg(vars, ray->orientation);
    d.yadd = (double)d.img->img_height / (double)d.linelen;
    ray->imgx = round(ray->imgx * ((double)d.img->img_width / 100.0)) * (d.img->bits_per_pixel / 8);
    d.screenx = round(raynum * 10);
    for (int j = d.y_start; j < d.y_end; j++)
    {
        d.color = *(unsigned int *)(d.img->addr + (int)((int)d.imgy * d.img->line_length) + (int)ray->imgx);
        mlx_pixel_put(vars->mlx, vars->win, d.screenx, j, d.color);
        mlx_pixel_put(vars->mlx, vars->win, d.screenx + 1, j, d.color);
        d.imgy += d.yadd;
    }
    mlx_put_image_to_window(vars->mlx, vars->win, vars->ceiling.img, d.screenx, d.y_start - 400);
    mlx_put_image_to_window(vars->mlx, vars->win, vars->floor.img, d.screenx, d.y_end);
    mlx_put_image_to_window(vars->mlx, vars->win, vars->ceiling.img, d.screenx+1, d.y_start - 400);
    mlx_put_image_to_window(vars->mlx, vars->win, vars->floor.img, d.screenx+1, d.y_end);
}

void ft_checkhit90(t_ray *ray, int **worldMap)
{
    if (worldMap[(int)ray->pos.y / 100][((int)ray->pos.x - 1) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 3;
        ray->imgx = 100.0 - fmod(ray->pos.x, 100.0);

    }
    else if (worldMap[((int)ray->pos.y - 1) / 100][(int)ray->pos.x / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 2;
        ray->imgx = fmod(ray->pos.y, 100.0);
    }
    else if (worldMap[((int)ray->pos.y) / 100][((int)ray->pos.x) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 2;
        ray->imgx = fmod(ray->pos.y, 100.0);
    }
}

void ft_checkhit180(t_ray *ray, int **worldMap)
{
    if (worldMap[(int)(ray->pos.y - 1) / 100][((int)ray->pos.x - 1) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 4;
        ray->imgx = 100.0 - fmod(ray->pos.y, 100.0);
    }
    else if (worldMap[(int)ray->pos.y / 100][((int)ray->pos.x) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 3;
        ray->imgx = 100.0 - fmod(ray->pos.x, 100.0);
    }
    else if (worldMap[(int)(ray->pos.y) / 100][(int)(ray->pos.x - 1) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 3;
        ray->imgx = 100.0 - fmod(ray->pos.x, 100.0);
    }
}

void ft_checkhit270(t_ray *ray, int **worldMap)
{
    if (worldMap[((int)ray->pos.y) / 100][((int)ray->pos.x - 1) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 4;
        ray->imgx = 100.0 - fmod(ray->pos.y, 100.0);
    }
    else if (worldMap[((int)ray->pos.y - 1) / 100][((int)ray->pos.x) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 1;
        ray->imgx = fmod(ray->pos.x, 100.0);
    }
    else if (worldMap[((int)ray->pos.y - 1) / 100][((int)ray->pos.x - 1) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 4;
        ray->imgx = 100.0 - fmod(ray->pos.y, 100.0);
    }
}

void ft_checkhit360(t_ray *ray, int **worldMap)
{
    if (worldMap[((int)ray->pos.y) / 100][(int)ray->pos.x / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 2;
        ray->imgx = fmod(ray->pos.y, 100.0);
    }
    else if (worldMap[((int)ray->pos.y - 1) / 100][(int)(ray->pos.x - 1) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 1;
        ray->imgx = fmod(ray->pos.x, 100.0);
    }
    else if (worldMap[(int)(ray->pos.y - 1) / 100][(int)(ray->pos.x) / 100] != 2)
    {
        ray->hit = 1;
        ray->orientation = 1;
        ray->imgx = fmod(ray->pos.x, 100.0);
    }
}
void ft_ray90(t_ray *ray, int **map)
{
    ray->rx = 100.0 - fmod(ray->pos.x, 100.0);
    ray->ry = 100.0 - fmod(ray->pos.y, 100.0);
    ray->hipx = ray->rx / fabs(cos(torad(ray->dir)));
    ray->hipy = ray->ry / fabs(sin(torad(ray->dir)));
    if (ray->hipx <= ray->hipy)
    {
        ray->pos.x += ray->rx;
        ray->pos.y += ray->hipx * fabs(sin(torad(ray->dir)));
    }
    else
    {
        ray->pos.y += ray->ry;
        ray->pos.x += ray->hipy * fabs(cos(torad(ray->dir)));
    }
    ft_checkhit90(ray, map);
}
void ft_ray180(t_ray *ray, int **map)
{
    ray->rx = fmod(ray->pos.x, 100.0);
    ray->ry = 100.0 - fmod(ray->pos.y, 100.0);
    if (ray->rx == 0)
        ray->rx = 100;
    ray->hipx = ray->rx / fabs(cos(torad(180 - ray->dir)));
    ray->hipy = ray->ry / fabs(sin(torad(180 - ray->dir)));
    if (ray->hipx <= ray->hipy)
    {
        ray->pos.x -= ray->rx;
        ray->pos.y += ray->hipx * fabs(sin(torad(180 - ray->dir)));
    }
    else
    {
        ray->pos.y += ray->ry;
        ray->pos.x -= ray->hipy * fabs(cos(torad(180 - ray->dir)));
    }
    ft_checkhit180(ray, map);
}
void ft_ray270(t_ray *ray, int **map)
{
    ray->rx = fmod(ray->pos.x, 100.0);
    ray->ry = fmod(ray->pos.y, 100.0);
    if (ray->rx == 0)
        ray->rx = 100;
    if (ray->ry == 0)
        ray->ry = 100;
    ray->hipx = ray->rx / fabs(cos(torad(ray->dir - 180)));
    ray->hipy = ray->ry / fabs(sin(torad(ray->dir - 180)));
    if (ray->hipx <= ray->hipy)
    {
        ray->pos.x -= ray->rx;
        ray->pos.y -= ray->hipx * fabs(sin(torad(ray->dir - 180)));
    }
    else
    {
        ray->pos.y -= ray->ry;
        ray->pos.x -= ray->hipy * fabs(cos(torad(ray->dir - 180)));
    }
    ft_checkhit270(ray, map);
}
void ft_ray360(t_ray *ray, int **map)
{
    ray->rx = 100.0 - fmod(ray->pos.x, 100.0);
    ray->ry = fmod(ray->pos.y, 100.0);
    if (ray->ry == 0)
        ray->ry = 100;
    ray->hipx = ray->rx / cos(torad(360 - ray->dir));
    ray->hipy = ray->ry / sin(torad(360 - ray->dir));
    if (ray->hipx <= ray->hipy)
    {
        ray->pos.x += ray->rx;
        ray->pos.y -= ray->hipx * sin(torad(360 - ray->dir));
    }
    else
    {
        ray->pos.y -= ray->ry;
        ray->pos.x += ray->hipy * cos(torad(360 - ray->dir));
    }
    ft_checkhit360(ray, map);
}

void rays_init(t_ray *ray, t_vars *vars)
{
    ray->pos.x = vars->player.pos.x;
    ray->pos.y = vars->player.pos.y;
    ray->hit = 0;
    ray->hip = 0;
}

int ft_rays(t_vars *vars)
{
    t_ray ray;

    for (float i = 0; i < 80.0f; i += 0.2f)
    {
        rays_init(&ray, vars);
        ray.dir = ft_fixangle(vars->player.dir - 40.0f + i);
        ray.planeangle = angleDifference(vars->player.dir, ray.dir);
        while (ray.hit == 0)
        {
            if (ray.dir < 90)
                ft_ray90(&ray, vars->map);
            else if (ray.dir < 180)
                ft_ray180(&ray, vars->map);
            else if (ray.dir < 270)
                ft_ray270(&ray, vars->map);
            else
                ft_ray360(&ray, vars->map);
            if (ray.hipx <= ray.hipy)
                ray.hip += ray.hipx;
            else
                ray.hip += ray.hipy;
        }
        ft_display(vars, &ray, i);
    }
    return (0);
}

void fill_image_with_color(t_image *img, unsigned int color)
{
    int y;
    char	*dst;

    for (y = 0; y < 400; y++)
    {
        dst = img->addr + (y * img->line_length);
        *(unsigned int *)dst = color;
    }
}
void ft_init(t_vars *v)
{
    v->ceilingcolor = 0;
    v->floorcolor = 0;
    v->player.pos.x = 0;
    v->player.pos.y = 0;
    v->player.dir = 0;
    v->buff = NULL;
    v->north.img = NULL;
    v->south.img = NULL;
    v->east.img = NULL;
    v->west.img = NULL;
    v->map = NULL;
    v->floor.img = NULL;
    v->ceiling.img = NULL;
    v->mlx = NULL;
    v->win = NULL;
}
int **createMap(int rows, int cols) {
    // Allocate memory for an array of row pointers
    int **map = (int **)malloc(rows * sizeof(int *));
    if (map == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    // Allocate memory for each row
    for (int i = 0; i < rows; i++) {
        map[i] = (int *)malloc(cols * sizeof(int));
        if (map[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");

            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(map[j]);
            }
            free(map);

            return NULL;
        }

        // Initialize each element to 1 if it's on the border, otherwise 0
        for (int j = 0; j < cols; j++) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                map[i][j] = 1;  // Border
            } else {
                map[i][j] = 0;  // Inside
            }
        }
    }

    return map;
}

void ft_checkfile(char *file)
{
    int i = 0;
    while (file[i])
        i++;
    if (file[i - 1] != 'b' || file[i - 2] != 'u' || file[i - 3] != 'c' || file[i - 4] != '.')
    {
        printf("Invalid file\n");
        exit(0);
    }
}

char    *readfd(char *file)
{
    int fd;
    char *buff;
    int bytes_read;

    fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        printf("Failed to open file\n");
        exit(0);
    }
    buff = malloc(BUFFER_SIZE + 1);
    bytes_read = read(fd, buff, BUFFER_SIZE);
    if (bytes_read == -1 || bytes_read == BUFFER_SIZE)
    {
        printf("Failed to read\n");
        free(buff);
        close(fd);
        exit(0);
    }
    close(fd);
    buff[bytes_read] = '\0';
    return (buff);
}

void *ft_calloc(size_t num_elements, size_t element_size)
{
    size_t total_size = num_elements * element_size;
    void* ptr = malloc(total_size);

    if (ptr != NULL)
    {
        char* char_ptr = (char*)ptr;
        size_t i = 0;
        while (i < total_size)
        {
            char_ptr[i] = 0;
            i++;
        }
    }
    return ptr;
}

char *substring(char *str, int start, int end)
{
    int length = end - start + 1;

    if (str == NULL || length < 0)
    {
        return NULL;
    }
    char *sub = (char *)ft_calloc(length + 1, sizeof(char));
    int i = 0;
    while (i < length) {
        sub[i] = str[start + i];
        i++;
    }
    sub[length] = '\0';
    return sub;
}

int ft_skipspaces(char *buff, int i)
{
    while (buff[i] == ' ' || buff[i] == '\t')
        i++;
    return (i);
}

int ft_put(t_image *img, char *buff, int i, t_vars *v)
{
    char *tmp;
    int j;

    if (img->img != NULL)
        ft_exit(v, "Double texture\n");
    i = ft_skipspaces(buff, i);
    j = i;
    while (buff[j] != '\n' && buff[j] != '\0')
        j++;
    tmp = substring(buff, i, j - 1);
    img->img = mlx_xpm_file_to_image(v->mlx, tmp, &img->img_width, &img->img_height);
    free(tmp);
    if (img->img == NULL)
        ft_exit(v, "Invalid texture\n");
    img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
    return (j);
}

int ft_isnumber(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

int ft_atoi(char *str)
{
    int i;
    int res;

    i = 0;
    res = 0;
    while (str[i])
    {
        if (ft_isnumber(str[i]) == 0)
        {
            return (res);
        }
        res = res * 10 + str[i] - '0';
        i++;
    }
    return (res);
}

int ft_loopcolor(char *buff, int i, t_vars *v)
{
    while (buff[i] && buff[i] != ',' && buff[i] != '\n')
    {
        if (ft_isnumber(buff[i]) == 0)
            ft_exit(v, "Invalid color\n");
        i++;
    }
    if (i == 0)
        ft_exit(v, "Invalid color\n");
    return (i);
}

void    ft_colorerr(char *buff)
{
    printf("Double color\n");
    free(buff);
    exit(0);
}

void    ft_insertcolor(unsigned int *color, int c[3], char *buff, t_vars *v)
{
    if (c[0] > 255 || c[1] > 255 || c[2] > 255)
        ft_exit(v, "Invalid color\n");
    *color = (c[0] << 16) | (c[1] << 8) | c[2];
}

int    ft_putcolor(unsigned int *color, char *buff, int i, t_vars *v)
{
    char *tmp;
    int c[3];
    int j;

    if (*color != 0)
        ft_colorerr(buff);
    i = ft_skipspaces(buff, i);
    j = ft_loopcolor(buff, i, v);
    tmp = substring(buff, i, j - 1);
    c[0] = ft_atoi(tmp);
    free(tmp);
    j++;
    i = j;
    j = ft_loopcolor(buff, i, v);
    tmp = substring(buff, i, j - 1);
    c[1] = ft_atoi(tmp);
    free(tmp);
    j++;
    i = j;
    j = ft_loopcolor(buff, i, v);
    tmp = substring(buff, i, j - 1);
    c[2] = ft_atoi(tmp);
    free(tmp);
    ft_insertcolor(color, c, buff, v);
    return (j);
}

int ft_parsevars(t_vars *vars, char *buff)
{
    int i;

    i = 0;
    while (buff[i])
    {
        if (buff[i] == 'N' && buff[i + 1] == 'O' && buff[i + 2] == ' ')
            i = ft_put(&vars->north, buff, i + 3, vars);
        else if (buff[i] == 'S' && buff[i + 1] == 'O' && buff[i + 2] == ' ')
            i = ft_put(&vars->south, buff, i + 3, vars);
        else if (buff[i] == 'E' && buff[i + 1] == 'A' && buff[i + 2] == ' ')
            i = ft_put(&vars->east, buff, i + 3, vars);
        else if (buff[i] == 'W' && buff[i + 1] == 'E' && buff[i + 2] == ' ')
            i = ft_put(&vars->west, buff, i + 3, vars);
        else if (buff[i] == 'F' && buff[i + 1] == ' ')
            i = ft_putcolor(&vars->floorcolor, buff, i + 2, vars);
        else if (buff[i] == 'C' && buff[i + 1] == ' ')
            i = ft_putcolor(&vars->ceilingcolor, buff, i + 2, vars);
        else if (buff[i] == '\n')
            i++;
        else
            return i;
    }
}

void ft_checkparse(t_vars *vars, char *buff)
{
    if (vars->north.img == NULL || vars->south.img == NULL || vars->east.img == NULL || vars->west.img == NULL)
        ft_exit(vars, "Missing texture\n");
    if (vars->floorcolor == 0 || vars->ceilingcolor == 0)
        ft_exit(vars, "Missing color\n");
}

int ft_countrows(char *buff, int i)
{
    int rows;

    rows = 0;
    while (buff[i])
    {
        if (buff[i] == '\n' || buff[i + 1] == '\0')
            rows++;
        i++;
    }
    return (rows);
}


int ft_checkifend(char *buff, int i)
{
    while (buff[i])
    {
        if (buff[i] != '\n')
            return (1);
        i++;
    }
    return (0);
}
int ft_allowedchar(char c)
{
    if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ' || c == '\n')
        return (1);
    return (0);
}

void ft_allocloop(t_vars *vars, char *buff, int rows, int i, int len)
{
    while (buff[i])
    {
        if (ft_allowedchar(buff[i]) == 0)
            ft_exit(vars, "Invalid map\n");
        if (buff[i] == '\n' || buff[i + 1] == '\0')
        {
            if (len == 0)
            {
                if (ft_checkifend(buff, i))
                    ft_exit(vars, "Invalid map\n");
                else
                    break;
            }
            if (buff[i + 1] == '\0')
                len++;
            vars->map[rows] = (int *)ft_calloc(sizeof(int), len + 2);
            len = -1;
            rows++;
        }
        len++;
        i++;
    }
}

void ft_allocmap(t_vars *vars, char *buff, int start)
{
    int i;
    int len;
    int rows;

    i = start;
    rows = ft_countrows(buff, i);
    vars->map = (int **)ft_calloc(sizeof(int *) , rows + 1);
    len = 0;
    rows = 0;
    ft_allocloop(vars, buff, rows, i, len);
}
void ft_setplayer(t_vars *vars, char c, int x, int y)
{
    if (vars->player.pos.x != 0 || vars->player.pos.y != 0)
        ft_exit(vars, "Double player detected. BANG! *BIG EXPLOSION*\n");
    vars->map[y][x] = 4;
    vars->player.pos.x = x * 100 + 50;
    vars->player.pos.y = y * 100 + 50;
    if (c == 'N')
        vars->player.dir = 270;
    else if (c == 'E')
        vars->player.dir = 0;
    else if (c == 'S')
        vars->player.dir = 90;
    else if (c == 'W')
        vars->player.dir = 180;
}

void ft_setmap(t_vars *vars, char *buff, int i)
{
    int x;
    int y;

    x = 0;
    y = 0;
    while (buff[i])
    {
        if (buff[i] == '\n')
        {
            y++;
            x = -1;
        }
        else if (buff[i] == ' ')
            vars->map[y][x] = 3;
        else if (buff[i] == '1')
            vars->map[y][x] = 1;
        else if (buff[i] == '0')
            vars->map[y][x] = 4;
        else if (buff[i] == 'N' || buff[i] == 'S' || buff[i] == 'E' || buff[i] == 'W')
            ft_setplayer(vars, buff[i], x, y);
        i++;
        x++;
    }
}

int ft_getmaph(int **map)
{
    int i;

    i = 0;
    while (map[i])
        i++;
    return (i);
}

int ft_getmapw(int **map)
{
    int i;

    i = 0;
    while (map[0][i])
        i++;
    return (i);
}

void ft_floodfill(t_vars *vars, int x, int y)
{
    int map_height = ft_getmaph(vars->map) + 1;
    int map_width = ft_getmapw(vars->map) + 1;

    if (x < 0 || x >= map_width || y < 0 || y >= map_height) 
        return;
    if (vars->map[y][x] == 4) 
    {
        vars->map[y][x] = 2;
        ft_floodfill(vars, x + 1, y);
        ft_floodfill(vars, x - 1, y);
        ft_floodfill(vars, x, y + 1);
        ft_floodfill(vars, x, y - 1);
    } 
    else if (vars->map[y][x] == 3 || vars->map[y][x] == 0) 
        ft_exit(vars, "Open map\n");
}


void    ft_parse(t_vars *vars, char *file)
{
    int i;

    ft_checkfile(file);
    vars->buff = readfd(file);
    vars->mlx = mlx_init();
    i = ft_parsevars(vars, vars->buff);
    ft_checkparse(vars, vars->buff);
    ft_allocmap(vars, vars->buff, i);
    ft_setmap(vars, vars->buff, i);
    if (vars->player.pos.x == 0 || vars->player.pos.y == 0)
        ft_exit(vars, "No player detected\n");
    free(vars->buff);
    vars->buff = NULL;
    ft_floodfill(vars, vars->player.pos.x / 100, vars->player.pos.y / 100);
}

void ft_printfmap(int **map)
{
    int i;
    int j;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            printf("%d", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

int main(int argc , char **argv)
{
    t_vars vars;

    if (argc != 2)
    {
        printf("Usage: ./cub3d map.cub\n");
        return (0);
    }
    ft_init(&vars);
    ft_parse(&vars, argv[1]);
    ft_printfmap(vars.map);
    vars.win = mlx_new_window(vars.mlx, WIDTH, HEIGHT, "Cuba3D");
    vars.ceiling.img = mlx_new_image(vars.mlx, 1, 400);
    vars.floor.img = mlx_new_image(vars.mlx, 1, 400);
    vars.ceiling.addr = mlx_get_data_addr(vars.ceiling.img, &vars.ceiling.bits_per_pixel, &vars.ceiling.line_length, &vars.ceiling.endian);
    vars.floor.addr = mlx_get_data_addr(vars.floor.img, &vars.floor.bits_per_pixel, &vars.floor.line_length, &vars.floor.endian);
    fill_image_with_color(&vars.ceiling, vars.ceilingcolor);
    fill_image_with_color(&vars.floor, vars.floorcolor);
    mlx_hook(vars.win, 2, 1L << 0, ft_key, &vars);
    mlx_loop_hook(vars.mlx, ft_rays, &vars);
    mlx_loop(vars.mlx);
    return (0);
}
