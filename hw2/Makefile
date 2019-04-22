CCBIN=/usr/bin/gcc
CC=$(CCBIN) -Wall -Wextra -Werror -pedantic -g -fsanitize=address
CFILES=crc32.c fingerprint.c generators.c packetsource.c stopwatch.c lamportQ.c

default: packets

packets: lib/*.h main.c serial.c parallel.c serialq.c $(CFILES)
	$(CC) -O3 -lm -lpthread -o packets main.c serial.c parallel.c serialq.c $(CFILES)

clean :
	rm -f packets
