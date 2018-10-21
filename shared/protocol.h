#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#if defined (__cplusplus)
extern "C" {
#endif

/* package header files */
#include <ti/ipc/MessageQ.h>

#include <stdint.h>

#define MSG_SIZE (sizeof(Message))

#define SHARED_REGION_ID_MSG_QUEUE   0x0
#define SHARED_REGION_ID_RING_BUFFER  0x1

#define RING_BUFFER_NAME      "RING_BUFFER"

#define MSG_QUEUE_NAME_HOST   "MSG_QUEUE_NAME_HOST"
#define MSG_QUEUE_NAME_SLAVE  "MSG_QUEUE_NAME_SLAVE"

typedef enum
{
    MSG_CMD_START = 0,

    MSG_CMD_ENABLE_ALGORISM,          //Enable Algorism
    MSG_CMD_DISABLE_ALGORISM,         //Disnable Algorism

    MSG_CMD_ALGORISM_DATA,
    MSG_CMD_RAW_DATA,

    MSG_CMD_CHA,                      //ADC Channel A
    MSG_CMD_CHB,                      //ADC Channel B

    MSG_CMD_ALGORISM1,
    MSG_CMD_ALGORISM2,

    MSG_CMD_RATE1,
    MSG_CMD_RATE2,
    MSG_CMD_RATE3,
    MSG_CMD_RATE4,
    MSG_CMD_RATE5,
    MSG_CMD_RATE6,
    MSG_CMD_RATE7,
    MSG_CMD_RATE8,
    MSG_CMD_RATE9,
    MSG_CMD_RATE10,
    MSG_CMD_RATE11,
    MSG_CMD_RATE12,
    MSG_CMD_RATE13,
    MSG_CMD_RATE14,
    MSG_CMD_RATE15,
    MSG_CMD_RATE16,
    MSG_CMD_RATE17,
    MSG_CMD_RATE18

} MsgCmd;

typedef struct _Message
{
    MessageQ_MsgHeader parent;
    uint32_t cmd; 
} Message;

#if defined (__cplusplus)
}
#endif

#endif
