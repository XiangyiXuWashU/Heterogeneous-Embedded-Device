#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <ti/syslink/RingIO.h>

#if defined __cplusplus
extern "C" {
#endif

struct _HostRingBuffer
{
    RingIO_Handle m_handle;
    RingIO_Handle m_read_handle;
    int m_alive;
};

typedef struct _HostRingBuffer HostRingBuffer;

HostRingBuffer *RingBuffer_new();
void            RingBuffer_destroy(HostRingBuffer *buffer);
void           *RingBuffer_acquired(HostRingBuffer *buffer,UInt32 size);
void            RingBuffer_release(HostRingBuffer *buffer,UInt32 size);
int             ipc_ring_buffer_check(HostRingBuffer *buffer,UInt32 size);
void            ipc_ring_buffer_size_check(HostRingBuffer *buffer,UInt32 size);

#if defined __cplusplus
}
#endif

#endif
