CC = gcc -fsanitize=thread
CFLAGS = -Wall -Wextra -Werror

SRC = main.c check.c threads.c utils.c init.c lifecycle.c init_threads.c
OBJ_DIR = OBJ
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
NAME = philo

all: $(NAME)

$(NAME): $(OBJ) Makefile
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.c Makefile philo.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -rf $(OBJ_DIR)

fclean :
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

re : fclean all

.PHONY : clean fclean re all
