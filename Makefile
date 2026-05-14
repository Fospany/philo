
SOURCE = 	philo.c \
			ft_atoi.c \
			threading.c \
			initializing.c

NAME = philo

CC = cc 

CFLAGS = -Wall -Wextra -Werror -g

HEADER = philo.h

OBJ = $(SOURCE:.c=.o)

all : $(NAME)
	@echo "Ready"

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(HEADER) -o $(NAME)

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY = all clean fclean re