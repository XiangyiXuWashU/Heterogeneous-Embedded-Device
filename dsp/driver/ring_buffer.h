#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#if defined __cplusplus
extern "C" {
#endif

typedef struct _RingBuffer RingBuffer;
struct _RingBuffer
{
    void   (* destroy)(RingBuffer *buffer);
    void * (* acquired)(RingBuffer *buffer, unsigned int size);
    void   (* release)(RingBuffer *buffer, unsigned int size);
};


void       ring_buffer_destroy(RingBuffer *buffer);


void     * ring_buffer_acquired(RingBuffer *buffer, unsigned int size);


void       ring_buffer_release(RingBuffer *buffer, unsigned int size);

#if defined __cplusplus
}
#endif
#endif
