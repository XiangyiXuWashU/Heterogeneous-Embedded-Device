#ifndef CONTROLER_H_
#define CONTROLER_H_

#include "../ipc/message_queue.h"

#if defined __cplusplus
extern "C" {
#endif

typedef struct _Controler Controler;

Controler *Controler_new(MessageQueue *queue);
void Controler_send_cmd(Controler *controler,uint32_t cmd);

#if defined __cplusplus
}
#endif

#endif
