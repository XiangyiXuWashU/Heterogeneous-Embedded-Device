#ifndef _AD_WORKSHOP_H_
#define _AD_WORKSHOP_H_

#include <ti/ipc/MessageQ.h>
#include "../dsp/driver/ring_buffer.h"

typedef struct _ADWorkshop ADWorkshop;

ADWorkshop * ad_workshop_new(RingBuffer *buffer, MessageQ_QueueId notify_queue, unsigned int heap_id);
void         ad_workshop_destroy(ADWorkshop *shop);
void         ad_workshop_do_algorism(ADWorkshop *shop, int flag);
void         ad_workshop_import(ADWorkshop *shop, float *data, unsigned int size);

#endif
