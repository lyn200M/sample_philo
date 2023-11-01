NAME = philo

SRCS =	main.c \
		helper.c \

CFLAGS =

OBJ = $(SRCS:.c=.o)

all : $(NAME)

$(NAME): $(OBJ)
		gcc -o $(NAME) $(CFLAGS) $(OBJ)
		echo "make philo success"

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)
