CCBIN=/usr/bin/gcc
CC=$(CCBIN) -Wall -Wextra -Werror -pedantic -g

default: floydwarshall

floydwarshall: lib/*.h main.c floydwarshall.c adj_mat.c
	$(CC) -O3 -lpthread -o floydwarshall main.c floydwarshall.c adj_mat.c

clean :
	rm -f floydwarshall
