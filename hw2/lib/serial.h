#ifndef SERIAL
#define SERIAL

#include "../provided/lib/fingerprint.h"
#include "../provided/lib/packetsource.h"
#include <assert.h>

void serial_experiment(PacketSource_t* packetsource, long T, char mode);

#endif
