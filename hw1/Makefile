CCBIN=/usr/bin/gcc
CC=$(CCBIN) -Wall -Wextra -Werror -pedantic -g

default: floydwarshall

floydwarshall: lib/*.h main.c floydwarshall.c adj_mat.c stopwatch.c
	$(CC) -O3 -lpthread -o floydwarshall main.c floydwarshall.c adj_mat.c stopwatch.c

clean :
	rm -f floydwarshall outfile outfile.csv parallel_out.txt serial_out.txt out.csv
