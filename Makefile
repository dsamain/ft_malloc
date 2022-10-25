
SRCS = src/malloc.c src/free.c

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
	export LD_LIBRARY_PATH=$(PWD):$(LD_LIBRARY_PATH)
	$(CC) -L$(PWD)/$(NAME) test.c $(NAME) -o test

clean:
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME) libft_malloc.so
	rm -f test

re:			fclean all

.PHONY:	all clean fclean re