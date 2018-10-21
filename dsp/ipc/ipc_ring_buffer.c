#include <stdlib.h>
#include <string.h>

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>

#include <ti/syslink/RingIO.h>
#include <ti/syslink/RingIOShm.h>
#include <ti/syslink/SysLink.h>

#include "../dsp/ipc/ipc_ring_buffer.h"

struct _IPCRingBuffer
{
    RingBuffer parent;
    RingIO_Handle handle;
    RingIO_Handle write_handle;  
};

RingBuffer *ipc_ring_buffer_new(char *name)
{
    IPCRingBuffer *buffer = (IPCRingBuffer *)malloc(sizeof(IPCRingBuffer));
    memset(buffer, 0, sizeof(IPCRingBuffer));

    buffer->parent.destroy = ipc_ring_buffer_destroy;
    buffer->parent.acquired = ipc_ring_buffer_acquired;
    buffer->parent.release = ipc_ring_buffer_release;

    SysLink_setup(); 
    RingIO_openParams params_open;
    params_open.openMode = RingIO_MODE_WRITER;
    params_open.flags  = 2; //Allow allocate non-continuous buffer

    //Open as write mode
    if (RingIO_open (name, &params_open, NULL, &buffer->write_handle) < 0)
    {
       Log_info0("RingIO_open fails");
    }

    else
    {
       Log_info0("RingIO_open successes");
    }

    return &buffer->parent;
}

void ipc_ring_buffer_destroy(RingBuffer *base)
{
    if (! base)
        return;

    IPCRingBuffer *buffer = (IPCRingBuffer *)base;

    RingIO_close(&buffer->write_handle);
    SysLink_destroy();
    free(buffer);
}

void *ipc_ring_buffer_acquired(RingBuffer *base, unsigned int size)
{
    IPCRingBuffer *buffer = (IPCRingBuffer *)base;
    char *buf;
    unsigned int actual = size;
    if (RingIO_acquire(buffer->write_handle, (RingIO_BufPtr* )&buf, &actual) != RingIO_S_SUCCESS)
    {
     	Log_info2("Wrong acquired, request is %d, actual is %d", size, actual);
        Log_info1("Empty size is %d", RingIO_getEmptySize(buffer->write_handle));
        Log_info1("Valid size is %d", RingIO_getValidSize(buffer->write_handle));
        return NULL;
    }

    return (void *)buf;
}


int ipc_ring_buffer_check(RingBuffer *base, unsigned int size)
{
	int status=0;
    IPCRingBuffer *buffer = (IPCRingBuffer *)base;

    if (RingIO_getEmptySize(buffer->write_handle)<size)
    {
       status=1;
    }

    return status;
}


void ipc_ring_buffer_release(RingBuffer *base, unsigned int size)
{
    IPCRingBuffer *buffer = (IPCRingBuffer *)base;

    if (RingIO_release(buffer->write_handle, size) < 0)
    {
      Log_error1("Fail to release buffer, size is %d", size);
    }
}
