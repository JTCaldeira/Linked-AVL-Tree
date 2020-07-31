CC		=	gcc
CFLAGS	=	-I -Wall -Wextra -Werror
OBJ		=	avl.o example.o
DEPS	=	avl.h


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


example: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


clean:
	rm $(OBJ) example
