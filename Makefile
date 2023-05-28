NAME = pipex
LIBFT = libft

SRCS = main.c \
	   child_process.c \


FLAGS = -Wextra -Werror -Wall
CC = @gcc ${FLAGS}
RM = @rm -f

OBJS = ${SRCS:.c=.o}

$(NAME): $(OBJS) 
	@make -C $(LIBFT)
	$(CC) $(OBJS) Libft/libft.a  -o $(NAME)

all: $(NAME)

clean:
	${RM} ${OBJS}
	@make clean -C $(LIBFT)

fclean: clean
	@make fclean -C $(LIBFT)
	${RM} ${NAME}
	
re: fclean all

.PHONY: all clean fclean re