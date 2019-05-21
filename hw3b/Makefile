CCBIN=/usr/bin/gcc
CCFAST=$(CCBIN) -Wall -Wextra -Werror -pedantic -g
CCDEBUG=$(CCFAST) -fsanitize=address -fno-omit-frame-pointer
CC=$(CCFAST)

FLAGS=-O3 -lm -lpthread -pthread

MY_CS=main.c serial.c lockfree.c homequeue.c awesome.c lamportQ.c timer.c lock.c
PROV_CS=provided/crc32.c provided/fingerprint.c provided/generators.c\
	provided/packetsource.c
LOCK_CS=locks/mutlock.c locks/alock.c

default: packets

packets: lib/*.h provided/lib/*.h locks/lib/*.h $(MY_CS) $(PROV_CS) $(LOCK_CS)
	$(CC) $(FLAGS) -o packets $(MY_CS) $(PROV_CS) $(LOCK_CS)

clean :
	rm -f packets
