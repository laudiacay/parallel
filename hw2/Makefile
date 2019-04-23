CCBIN=/usr/bin/gcc
CC=$(CCBIN) -Wall -Wextra -Werror -pedantic -g -fsanitize=address
FLAGS=-O3 -lm -lpthread -pthread
PROV_CFILES=provided/crc32.c provided/fingerprint.c provided/generators.c\
	provided/packetsource.c provided/stopwatch.c
MY_CFILES=main.c serial.c parallel.c serialq.c lamportQ.c

default: packets

packets: lib/*.h provided/lib/*.h $(MY_CFILES) $(PROV_CFILES)
	$(CC) $(FLAGS) -o packets $(MY_CFILES) $(PROV_CFILES)

clean :
	rm -f packets
