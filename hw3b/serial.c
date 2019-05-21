#include <pthread.h>

#include "lib/timer.h"

#include "provided/lib/packetsource.h"
#include "provided/lib/fingerprint.h"

volatile int ser_should_quit = 0;

struct ser_worker_args {
    volatile Packet_t* (*getPacket)(PacketSource_t*, int);
    PacketSource_t* packetsource;
};

void* do_serial(void* v_ser_was) {
    struct ser_worker_args* ser_was = (struct ser_worker_args*) v_ser_was;

    PacketSource_t* packetsource = ser_was->packetsource;
    volatile Packet_t* (*getPacket)(PacketSource_t*, int) = ser_was->getPacket;
    volatile Packet_t* packet;

    // keep going until timer expires
    while (!ser_should_quit) {
        // iterate over all sources
        for (int s_num = 0; s_num < packetsource->numSources; s_num++) {
            packet = getPacket(packetsource, s_num);
            // throwing out the checksum, might use later
            getFingerprint(packet->iterations, packet->seed);
            // other threads are definitely done w/ packet at this point
            // so we can cast away the volatile and free it
            free((void*) packet);
        }
    }

    return NULL;
}

void serial_exp(PacketSource_t* packetsource,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m) {

    struct ser_worker_args ser_was = {getPacket, packetsource};

    pthread_t thread;

    pthread_create(&thread, NULL, &do_serial, (void*) &ser_was);
    timer(&ser_should_quit, m);
    pthread_join(thread, NULL);
}
