NAME = libvds

SRC = $(wildcard lib/*.c)

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	@ar rc $(NAME).a $(OBJ)
	@ranlib $(NAME).a

clean:
	@$(RM) $(OBJ)

fclean: clean
	@$(RM) $(NAME).a

re: fclean all
