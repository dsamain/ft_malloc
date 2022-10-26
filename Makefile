
SRCS = $(addprefix src/, malloc.c free.c realloc.c show_alloc_mem.c calloc.c)

OBJS = $(SRCS:.c=.o)

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LIB_NAME := libft_malloc.so

CC = gcc

CFLAGS = -fPIC  #-fsanitize=address # -Wall -Wextra -Werror

OS := $(shell uname)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	gcc -shared -o $(NAME) $(OBJS)
	ln -s $(NAME) $(LIB_NAME)

all:		$(NAME)
    

test: all
	export LD_LIBRARY_PATH=$(PWD)
	$(CC) -L$(PWD)/$(LIB_NAME) test.c $(LIB_NAME) -o test

std: all
	export LD_PRELOAD=$(PWD)/$(LIB_NAME)
	$(CC) test.c -D STD -o std

clean:
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME) $(LIB_NAME)
	rm -f test std

re:			fclean all

.PHONY:	all clean fclean re