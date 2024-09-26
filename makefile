CC=gcc
CFLAGS += -W -Wall -ansi -pedantic -std=c99 -g -Iinclude
SRC=src/
NAME= astarc

FILES	=	$(SRC)main.c \
			$(SRC)get_file_content.c \
			$(SRC)get_map_content.c \
			$(SRC)astar.c \
			$(SRC)node_utils.c \
			$(SRC)hash_table.c \
			$(SRC)binary_heap.c \
			$(SRC)debug.c

OBJ		=	$(FILES:.c=.o)

$(NAME): $(OBJ)
		gcc -o $(NAME) $(OBJ) -lm

all:     $(NAME)

clean:
		rm -f src/*.o	

fclean: clean
		rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re