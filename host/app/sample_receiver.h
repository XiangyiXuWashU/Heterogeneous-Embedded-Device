#ifndef SAMPLE_RECEIVER_H_
#define SAMPLE_RECEIVER_H_

#include "../app/thread.h"
#include "../ipc/ring_buffer_host.h"
#include "../ipc/message_queue.h"
#include "../network/wifi.h"

#if defined __cplusplus
extern "C" {
#endif

struct _SampleReceiver
{
    volatile int m_running;      //SampleReceiver running flag
    HostRingBuffer *m_buffer;    //RingIO
    MessageQueue *m_msg_queue;   //Send and receive MessageQ between two processors
    Udp    *m_udp;               //WIFI UDP
};
typedef struct _SampleReceiver SampleReceiver;

typedef struct
{
    unsigned short	     front;             //Queue Front
    unsigned short	     rear;	        //Queue Rear
    float	             sample_buff[1024]; //WIFI Send Buffer
}SAMPLEQUEUE;

SampleReceiver *SampleReceiver_new(HostRingBuffer *buffer, MessageQueue *queue,Udp *udp);
void SampleReceiver_destroy(SampleReceiver *receiver, Thread *thread);
void SampleReceiver_start(SampleReceiver *receiver,Thread *thread);
void SampleReceiver_stop(SampleReceiver *receiver);
void *SampleReceiver_run(void *arg);
void *Wifi_sending(void *arg);

#if defined __cplusplus
}
#endif

#endif
