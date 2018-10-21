#ifndef _IPC_RING_BUFFER_H_
#define _IPC_RING_BUFFER_H_

#include "../dsp/driver/ring_buffer.h"

#if defined __cplusplus
extern "C" {
#endif

typedef struct _IPCRingBuffer IPCRingBuffer;

RingBuffer * ipc_ring_buffer_new(char *name);
void         ipc_ring_buffer_destroy(RingBuffer *buffer);
void       * ipc_ring_buffer_acquired(RingBuffer *buffer, unsigned int size);
int           ipc_ring_buffer_check(RingBuffer *base, unsigned int size);
void         ipc_ring_buffer_release(RingBuffer *buffer, unsigned int size);

#if defined __cplusplus
}
#endif
#endif
