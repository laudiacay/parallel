#ifndef SERIALQ
#define SERIALQ

#include "lamportQ.h"
#include "packetsource.h"

void serialq_experiment(PacketSource_t* packetsource, long T, int n, char mode,
        struct WaitFreeQueue** qs);

#endif
