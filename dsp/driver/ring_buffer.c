#include "../dsp/driver/ring_buffer.h"

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>

void ring_buffer_destroy(RingBuffer *buffer)
{
    buffer->destroy(buffer);
}

void *ring_buffer_acquired(RingBuffer *buffer, unsigned int size)
{
    return buffer->acquired(buffer, size);
}

void ring_buffer_release(RingBuffer *buffer, unsigned int size)
{
    buffer->release(buffer, size);
}
