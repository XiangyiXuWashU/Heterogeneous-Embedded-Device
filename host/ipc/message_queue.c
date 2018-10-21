#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ti/syslink/Std.h>     /* must be first */
#include <ti/syslink/IpcHost.h>
#include <ti/syslink/SysLink.h>

#include <ti/ipc/MultiProc.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/SharedRegion.h>

#include <ti/syslink/ProcMgr.h>
#include <ti/syslink/utils/IHeap.h>
#include <ti/syslink/utils/Memory.h>

#include "../ipc/message_queue.h"
#include "../ipc/message_heap.h"
#include "../shared/protocol.h"
#include "../host/app/log.h"

struct _MessageQueue
{
    MessageHeap *m_heap;
    uint32_t m_heap_id;
    MessageQ_Handle m_queue_recv;   //MessageQ_Handle
    MessageQ_QueueId m_queue_send;  //MessageQ_QueueId
};


MessageQueue *message_queue_new()
{
    //Use the MessageQueue module to define a variable and allocate the memory
    MessageQueue *queue = (MessageQueue *)malloc(sizeof(MessageQueue));
    memset(queue, 0, sizeof(MessageQueue));

    //Create a MessageHeap
    queue->m_heap = message_heap_new(SHARED_REGION_ID_MSG_QUEUE);
    queue->m_heap_id = message_heap_id(queue->m_heap);

    //Create a MessageQ
    MessageQ_Params params;
    MessageQ_Params_init(&params);
    queue->m_queue_recv = MessageQ_create(MSG_QUEUE_NAME_HOST, &params);

    //It will be created by the slave
    queue->m_queue_send = MessageQ_INVALIDMESSAGEQ;

    //Open a MessageQ given the queue index and remote processor id
    int status;
    do {
        status = MessageQ_open(MSG_QUEUE_NAME_SLAVE, &queue->m_queue_send);
        if (status == MessageQ_E_NOTFOUND)
            sleep(1);
    } while (status == MessageQ_E_NOTFOUND);

    if (status < 0)
    {
    	LOG_FATAL("Failed to MessageQ %s",MSG_QUEUE_NAME_HOST);
    }

    return queue;
}

void message_queue_destroy(MessageQueue *queue)
{
    MessageQ_delete(&queue->m_queue_recv);
    MessageQ_close(&queue->m_queue_send);
    message_heap_destroy(queue->m_heap);
}

MessageQ_Msg msg_alloc(MessageQueue *queue,UInt32 size)
{
    MessageQ_Msg msg = MessageQ_alloc(queue->m_heap_id, size);

    if (msg == NULL)
    {
    	LOG_FATAL("Failed to MessageQ_alloc");
    }

    return msg;
}

void msg_free(void *msg)
{
    MessageQ_free((MessageQ_Msg)msg);
}

//Send message to slave
void push(MessageQueue *queue,void *msg)
{
    MessageQ_setReplyQueue(queue->m_queue_recv, (MessageQ_Msg)msg);
    MessageQ_put(queue->m_queue_send, (MessageQ_Msg)msg);
}

//Receive message from slave
MessageQ_Msg pop(MessageQueue *queue,UInt32 timeout_msec)
{
    MessageQ_Msg msg = NULL;
    MessageQ_get(queue->m_queue_recv, &msg, timeout_msec);
    return msg;
}
