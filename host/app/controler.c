#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <ti/syslink/Std.h>     /* must be first */
#include <ti/ipc/MessageQ.h>

#include "../app/controler.h"
#include "../ipc/message_queue.h"
#include "../shared/protocol.h"

struct _Controler
{
    MessageQueue *m_queue;
};

Controler *Controler_new(MessageQueue *queue)
{
    Controler *controler = (Controler *)malloc(sizeof(Controler));
    memset(controler, 0, sizeof(Controler));

    //Initialization
    controler->m_queue = queue;

    return controler;
}

//Send Command to MessageQ
void Controler_send_cmd(Controler *controler,uint32_t cmd)
{
    Message *msg = (Message *)msg_alloc(controler->m_queue,MSG_SIZE);
    msg->cmd = cmd;
    push(controler->m_queue,msg);
}


