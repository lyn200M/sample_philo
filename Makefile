# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lnyamets <lnyamets@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/01 23:13:00 by lnyamets          #+#    #+#              #
#    Updated: 2023/11/01 23:18:38 by lnyamets         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS =	main.c \
		helper.c \
		simulation_util.c \
		eating.c \

CFLAGS = -g -Werror -Wextra -Wall -pthread

OBJ = $(SRCS:.c=.o)

all : $(NAME)

$(NAME): $(OBJ)
		cc -o $(NAME) $(CFLAGS) $(OBJ)
		echo "make philo success"

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)
re: fclean all
