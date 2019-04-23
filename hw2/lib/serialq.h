#ifndef SERIALQ
#define SERIALQ

#include "lamportQ.h"
#include "../provided/lib/packetsource.h"
#include "../provided/lib/fingerprint.h"
#include <assert.h>

void serialq_experiment(PacketSource_t* packetsource, long T, int n, char mode,
        struct WaitFreeQueue** qs);

#endif
