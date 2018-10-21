#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <ti/syslink/Std.h>     /* must be first */
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/MessageQ.h>
#include <ti/syslink/utils/IHeap.h>

#include "../ipc/message_heap.h"
#include "../host/app/log.h"

MessageHeap * message_heap_new(unsigned int shared_region_id)
{
    MessageHeap *heap = (MessageHeap *)malloc(sizeof(MessageHeap));
    memset(heap, 0, sizeof(MessageHeap));

    heap->heap = (IHeap_Handle)SharedRegion_getHeap(shared_region_id);
    heap->id = 1;

    if (heap->heap == NULL)
    {
    	LOG_INFO("Heap is null");
    }


    if (MessageQ_registerHeap((Ptr)(heap->heap), heap->id) < 0)
    {
    	LOG_INFO("Failed to register heap");
    }

    return heap;
}

void message_heap_destroy(MessageHeap *heap)
{
    if (! heap)
        return;

    MessageQ_unregisterHeap(heap->id);
    free(heap);
}

unsigned int message_heap_id(MessageHeap *heap)
{
    return heap->id;
}
