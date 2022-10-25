
SRCS = $(addprefix src/, malloc.c)

OBJS = $(SRCS:.c=.o)

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so

CC = gcc

CFLAGS = -fPIC  #-fsanitize=address # -Wall -Wextra -Werror

OS := $(shell uname)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	gcc -shared -o $(NAME) $(OBJS)


all:		$(NAME)

test: all
	$(CC) -o test test.c -L $(NAME) -fsanitize=address

clean:
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME) libft_malloc.so

re:			fclean all

.PHONY:	all clean fclean re