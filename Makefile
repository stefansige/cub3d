NAME = cub3d
CC = gcc
CFLAGS = -Wall -Werror -Wextra
SRC = main.c utils.c
OBJ = $(SRC:.c=.o)

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
