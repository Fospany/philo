SOURCE = 	philo.c \
			ft_atoi.c \
			routine.c \
			initializing.c \
			threads_and_monitoring.c \
			monitoring_helpers.c \
			routine_helpers.c \

NAME = philo

CC = cc 

CFLAGS = -Wall -Wextra -Werror -g

OBJ = $(SOURCE:.c=.o)

all : $(NAME)
	@echo "Ready"

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY = all clean fclean re