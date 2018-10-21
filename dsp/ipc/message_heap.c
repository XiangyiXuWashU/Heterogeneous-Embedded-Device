#include <stdlib.h>
#include <string.h>

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>

#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/MessageQ.h>
#include <ti/syslink/utils/IHeap.h>

#include "../dsp/ipc/message_heap.h"

struct _MessageHeap
{
    IHeap_Handle heap;
    unsigned int id;
};


MessageHeap * message_heap_new(unsigned int shared_region_id)
{
    MessageHeap *heap = (MessageHeap *)malloc(sizeof(MessageHeap));
    memset(heap, 0, sizeof(MessageHeap));

    heap->heap = (IHeap_Handle)SharedRegion_getHeap(shared_region_id);
    heap->id = 1;

    if (heap->heap == NULL)
    	Log_error0("Fail to get heap in SharedRegion");

    if (MessageQ_registerHeap((Ptr)(heap->heap), heap->id) < 0)
    	Log_error0("Failed to register heap");

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
