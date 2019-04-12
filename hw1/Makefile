CCBIN=/usr/bin/gcc
CC=$(CCBIN) -Wall -Wextra -Werror -pedantic -g -fsanitize=address -fno-omit-frame-pointer

default: floydwarshall

floydwarshall: lib/*.h serial.c floydwarshall.c adj_mat.c
	$(CC) -O3 -o floydwarshall floydwarshall.c serial.c adj_mat.c

clean :
	rm -f floydwarshall
