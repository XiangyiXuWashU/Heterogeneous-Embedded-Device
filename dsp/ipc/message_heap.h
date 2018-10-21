#ifndef _MESSAGE_HEAP_H_
#define _MESSAGE_HEAP_H_

#if defined __cplusplus
extern "C" {
#endif

typedef struct _MessageHeap MessageHeap;

MessageHeap * message_heap_new(unsigned int shared_region_id);
void          message_heap_destroy(MessageHeap *heap);
unsigned int  message_heap_id(MessageHeap *heap);

#if defined __cplusplus
}
#endif
#endif
