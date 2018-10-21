#ifndef _AD9238_H_
#define _AD9238_H_

#include "../dsp/driver/ring_buffer.h"

 //Define Call-back Function
typedef void (* TRIGGER_CALLBACK)(float *data, void *user_data);

typedef struct _ad9238 ad9238;

//Call-back Event
void     ad9238_trigger(ad9238 *ad, TRIGGER_CALLBACK cb, void *user_data);
void     judgeADCoutofRange(unsigned short inputA, unsigned short inputB);
ad9238 * ad9238_new(RingBuffer *buffer);
void     startuPPTask(ad9238 *ad);
void     ad9238_destroy(ad9238 *ad);
void     ad9238_start(ad9238 *ad, unsigned int rate);

#endif
