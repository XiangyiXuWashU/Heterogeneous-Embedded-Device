#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <ti/syslink/Std.h>     /* must be first */
#include <ti/syslink/IpcHost.h>
#include <ti/syslink/SysLink.h>
#include <ti/syslink/RingIO.h>
#include <ti/syslink/RingIOShm.h>

#include <ti/ipc/MultiProc.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/SharedRegion.h>

#include <ti/syslink/ProcMgr.h>
#include <ti/syslink/utils/IHeap.h>
#include <ti/syslink/utils/Memory.h>

#include "../ipc/ring_buffer_host.h"
#include "../shared/protocol.h"
#include "../host/app/log.h"


HostRingBuffer *RingBuffer_new()
{
    //Use the RingBuffer module to define a variable and allocate the memory
    HostRingBuffer *buffer = (HostRingBuffer *)malloc(sizeof(HostRingBuffer));
    memset(buffer, 0, sizeof(HostRingBuffer));

    //Create the RingIO
    RingIOShm_Params  params_create;
    RingIOShm_Params_init(&params_create);
    params_create.commonParams.name     = RING_BUFFER_NAME;
    params_create.ctrlRegionId          = SHARED_REGION_ID_RING_BUFFER;
    params_create.dataRegionId          = SHARED_REGION_ID_RING_BUFFER;
    params_create.attrRegionId          = SHARED_REGION_ID_RING_BUFFER;
    params_create.attrSharedAddrSize    = 16;
    params_create.dataSharedAddrSize    = 0xE00000; //14MB Data
    params_create.remoteProcId          = MultiProc_getId("DSP");
    params_create.gateHandle            = NULL;

    buffer->m_handle = RingIO_create(&params_create);

    if(buffer->m_handle == NULL)
    {
    	LOG_ERROR("Fail to create RingIO %s", RING_BUFFER_NAME);
    	buffer->m_alive = 0;
    }
    else
    {
    	//Open the RingIO
        RingIO_openParams  params_open;
        params_open.openMode = RingIO_MODE_READER;
        params_open.flags = 0;
        if (RingIO_open (RING_BUFFER_NAME, &params_open, NULL, &buffer->m_read_handle) < 0)
        {
        	LOG_ERROR("Fail to RingIO_open");
        	buffer->m_alive = 0;
        }
        else
        {
            LOG_DEBUG("%s is ready",RING_BUFFER_NAME);
            buffer->m_alive = 1;
        }

    }

    return buffer;
}

void  RingBuffer_destroy(HostRingBuffer *buffer)
{
    if (buffer->m_alive)
    {
        RingIO_close(&buffer->m_read_handle);
        RingIO_delete(&buffer->m_handle);
    }
}

void *RingBuffer_acquired(HostRingBuffer *buffer,UInt32 size)
{
    if (! buffer->m_alive)
    {
        LOG_WARN("Ring buffer is not alive");
        return NULL;
    }

    uint8_t *buf;
    if (RingIO_acquire(buffer->m_read_handle, (RingIO_BufPtr*)&buf, &size) != RingIO_S_SUCCESS)
    {
        LOG_ERROR("Wrong acquired, Max is %d", size);
        return NULL;
    }

    return (void *)buf;
}

void  RingBuffer_release(HostRingBuffer *buffer,UInt32 size)
{
    if (! buffer->m_alive)
    {
    	LOG_WARN("Ring buffer is not alive");
        return;
    }

    if (RingIO_release(buffer->m_read_handle, size) < 0)
    {
    	LOG_ERROR("Failed to RingIO_release");
    }
}

int ipc_ring_buffer_check(HostRingBuffer *buffer,UInt32 size)
{
    int status=0;

    if (RingIO_getValidSize(buffer->m_read_handle)<size)
    {
       status=1;
    }

    return status;
}

void ipc_ring_buffer_size_check(HostRingBuffer *buffer,UInt32 size)
{
    if (RingIO_getEmptySize(buffer->m_read_handle)<size)
    {
    	LOG_INFO("Empty size is %d", RingIO_getEmptySize(buffer->m_read_handle));
    	LOG_INFO("Valid size is %d", RingIO_getValidSize(buffer->m_read_handle));
    }
}

