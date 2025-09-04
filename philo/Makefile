NAME=philo
SRC= main.c utils.c init.c events.c just_atoi.c
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