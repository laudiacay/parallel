CCBIN=/usr/bin/gcc
CC=$(CCBIN) -Wall -Wextra -Werror -pedantic -g -std=c11 -fsanitize=address -fno-omit-frame-pointer
FLAGS=-O3 -lm -lpthread -pthread
PROV_CFILES=provided/stopwatch.c
MY_CFILES=main.c serial.c parallel.c lock.c lock_src/taslock.c \
		lock_src/ttaslock.c lock_src/mutlock.c lock_src/alock.c lock_src/clhlock.c

default: locks

locks: lib/*.h provided/lib/*.h lock_src/lib/*.h $(MY_CFILES) $(PROV_CFILES)
	$(CC) $(FLAGS) -o locks $(MY_CFILES) $(PROV_CFILES)

clean :
	rm -f locks
