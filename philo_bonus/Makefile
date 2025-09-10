NAME=philo_bonus
SRC= main_bonus.c
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Wextra -Werror -pthread
CC=cc

all:$(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re