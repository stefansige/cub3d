#include <mlx.h>
#include <math.h> // Include math.h for abs function
#include <stdlib.h>

#define screenWidth 640
#define screenHeight 480
#define mapWidth 7
#define mapHeight 7

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,3,1,1,1},
  {1,0,0,0,0,0,1},
  {1,0,0,0,0,0,1},
  {1,0,0,0,0,0,2},
  {1,0,0,0,0,0,1},
  {1,0,0,0,0,0,1},
  {1,1,1,2,1,1,1}
};

void draw_line(int x1, int y1, int x2, int y2, void *mlx_ptr, void *win_ptr, int color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float Xincrement = dx / (float) steps;
    float Yincrement = dy / (float) steps;
    float X = x1;
    float Y = y1;
    for (int i = 0; i <= steps; i++)
    {
        mlx_pixel_put(mlx_ptr, win_ptr, round(X), round(Y), color);
        X += Xincrement;
        Y += Yincrement;
    }
}

int main()
{
    void *mlx_ptr;
    void *win_ptr;

    mlx_ptr = mlx_init();
    win_ptr = mlx_new_window(mlx_ptr, screenWidth, screenHeight, "Raycaster");



	double posX = 3, posY = 3;      // x and y start position
	double dirX = 1, dirY = 0;     // initial direction vector (facing west)
	double planeX = 0, planeY = 0;  // the 2D raycaster version of camera plane


    while(1)
    {
        for(int x = 0; x < screenWidth; x++)
        {
            // Calculate ray direction for this column
            double cameraX = 2 * x / (double)screenWidth - 1;
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            // Map position
            int mapX = (int)posX;
            int mapY = (int)posY;

            // Length of ray from current position to next x or y-side
            double sideDistX, sideDistY;

            // Length of ray from one x or y-side to next x or y-side
            double deltaDistX = fabs(1 / rayDirX);
            double deltaDistY = fabs(1 / rayDirY);
            double perpWallDist;

            // Direction to step in x and y-direction
            int stepX, stepY;

            int hit = 0; // Was there a wall hit?
            int side; // Was a NS or a EW wall hit?

            // Calculate step and initial sideDist
            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }

            // Perform DDA
            while (hit == 0)
            {
                // Jump to next map square, either in x-direction or y-direction
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                // Check if ray has hit a wall
                if (mapX >= 0 && mapX < mapWidth && mapY >= 0 && mapY < mapHeight && worldMap[mapX][mapY] > 0)
      				hit = 1;
            }

            // Calculate distance projected on camera direction
            if (side == 0)
                perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
            else
                perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

            // Calculate height of line to draw on screen
            int lineHeight = (int)(screenHeight / perpWallDist);

            // Calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + screenHeight / 2;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if (drawEnd >= screenHeight)
                drawEnd = screenHeight - 1;

            // Choose wall color
            int color = worldMap[mapX][mapY] == 1 ? 0xFF0000 : 0x00FF00; // Red for wall, green for empty space

            // Give x and y sides different brightness
            if (side == 1)
                color = color / 2;

            // Draw the pixels of the stripe as a vertical line
            draw_line(x, drawStart, x, drawEnd, mlx_ptr, win_ptr, color);
        }

        mlx_loop(mlx_ptr);
    }

    return 0;
}
