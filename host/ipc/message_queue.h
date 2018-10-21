#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#include <ti/ipc/MessageQ.h>

#if defined __cplusplus
extern "C" {
#endif

typedef struct _MessageQueue MessageQueue;

MessageQueue *message_queue_new();
void          message_queue_destroy(MessageQueue *queue);
MessageQ_Msg  msg_alloc(MessageQueue *queue,UInt32 size);
void          msg_free(void *msg);
void          push(MessageQueue *queue,void *msg);
MessageQ_Msg  pop(MessageQueue *queue,UInt32 timeout_msec);

#if defined __cplusplus
}
#endif


#endif
