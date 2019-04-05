CCBIN=/usr/bin/gcc
CC=$(CCBIN) -Wall -Wextra -Werror -std=c99 -pedantic -g -fsanitize=address
C_LIBS=
LINK_LIBS=

default: floydwarshall

floydwarshall: $(C_LIBS) floydwarshall.c
	$(CC) -O3 -o floydwarshall $(C_LIBS) floydwarshall.c $(LINK_LIBS)

clean :
	rm floydwarshall
