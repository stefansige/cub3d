NAME = cub3d
CC = gcc
CFLAGS = -g
LDLIBS = -L./minilibx-linux -lmlx_Linux -lXext -lX11 -lm
INCLUDES = -I./minilibx-linux
SRC = main.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)

%.o: %.c
		$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all
