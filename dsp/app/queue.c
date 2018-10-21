#include <stdlib.h>
#include <string.h>

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include "../dsp/app/queue.h"

struct _Queue {
    Queue_Handle handle; 
    Semaphore_Handle sem;
};

Queue *queue_new() {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    memset(queue, 0, sizeof(Queue));

    queue->handle = Queue_create(NULL, NULL);
    if (! queue->handle)
        Log_error0("failed to create queue");


    queue->sem = Semaphore_create(0, NULL, NULL);
    if (! queue->sem)
        Log_error0("failed to create semaphore");

    return queue;
}

void queue_destroy(Queue *queue)
{
    if (! queue)
        return;

    Queue_delete(&queue->handle);
    Semaphore_delete(&queue->sem);
    free(queue);
}

void queue_push(Queue *queue, Queue_Elem *elm)
{
    Queue_put(queue->handle, elm);  
    Semaphore_post((Semaphore_Object *)queue->sem); /* notity */
}

Queue_Elem *queue_pop(Queue *queue)
{
    while (Queue_empty(queue->handle))
        Semaphore_pend(queue->sem, BIOS_WAIT_FOREVER);

    return Queue_get(queue->handle);
}
