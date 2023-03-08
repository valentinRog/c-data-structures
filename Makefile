NAME = libvds.so

SRC = $(wildcard lib/*.c)

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror

CC = gcc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJ)
clean:
	@$(RM) $(OBJ)

fclean: clean
	@$(RM) $(NAME)

re: fclean all
