#ifndef _MESSAGE_LOOP_H_
#define _MESSAGE_LOOP_H_

#include "../dsp/driver/ad9238.h"

#if defined __cplusplus
extern "C" {
#endif

typedef struct _MessageLoop MessageLoop;

MessageLoop * message_loop_new();
void          message_loop_start(MessageLoop *loop);
void          message_loop_destroy(MessageLoop *loop);
void          message_loop_stop(MessageLoop *loop);


#if defined __cplusplus
}
#endif
#endif
