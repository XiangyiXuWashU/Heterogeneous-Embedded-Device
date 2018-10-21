#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include <ti/syslink/Std.h>     /* must be first */
#include <ti/syslink/IpcHost.h>
#include <ti/syslink/SysLink.h>
#include <ti/syslink/RingIO.h>
#include <ti/syslink/RingIOShm.h>
#include <ti/syslink/ProcMgr.h>
#include <ti/syslink/utils/IHeap.h>
#include <ti/syslink/utils/Memory.h>

#include <ti/ipc/MultiProc.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/SharedRegion.h>

#include <pthread.h>

#include <sys/time.h>

#include "../app/sample_receiver.h"
#include "../app/thread.h"

#include "../ipc/ring_buffer_host.h"
#include "../ipc/message_queue.h"
#include "../network/wifi.h"
#include "../shared/protocol.h"
#include "../host/app/log.h"

int protect = 0;         //Protect samples while sending
float samples[1024];     //used for float data from ring buffer

SAMPLEQUEUE wifisample;

SampleReceiver *SampleReceiver_new(HostRingBuffer *buffer, MessageQueue *queue,Udp *udp)
{
    SampleReceiver *receiver = (SampleReceiver *)malloc(sizeof(SampleReceiver));
    memset(receiver, 0, sizeof(SampleReceiver));

    //Initialization
    receiver->m_msg_queue = queue;
    receiver->m_running   = 0;
    receiver->m_buffer    = buffer;
    receiver->m_udp       = udp;

    return receiver;
}

void  SampleReceiver_destroy(SampleReceiver *receiver, Thread *thread)
{
    receiver->m_running = 0;

    //Detach current thread and Wait for another thread to perform PTHREAD_JOIN
    Thread_destroy(thread);
}


void SampleReceiver_start(SampleReceiver *receiver,Thread *thread)
{
    receiver->m_running = 1;
    thread_start(thread);
}

void SampleReceiver_stop(SampleReceiver *receiver)
{
    receiver->m_running = 0;
}

//Push into queue
void wifisample_rx_enqueue(float x)
{
    wifisample.sample_buff[wifisample.rear] = x;
    wifisample.rear = wifisample.rear+1;
}

//Pop from queue
float wifisample_rx_dequeue(void)
{
    float temp;

    temp = wifisample.sample_buff[wifisample.front];
    wifisample.front = wifisample.front+1;
    return temp;
}

//Receive data from DSP
//sample_receiver_thread
void *SampleReceiver_run(void *arg)
{
    LOG_INFO("SampleReceiver starts running");

    int i=0;

    UInt32 size = sizeof(float)*1024*1;

    SampleReceiver *receiver = (SampleReceiver *)arg;

    while (receiver->m_running)
    {
        //Wait for slave, timeout is 2 second
        Message *msg = (Message *)pop(receiver->m_msg_queue,2000);

        //Judge whether it should quit now
        if (! receiver->m_running)
        {
            if (msg)
            	msg_free(msg);
            break;
        }

        //Judge timeout
        if (! msg)
        {
            LOG_TRACE("Receive data timeout");
            continue;
        }

        //Acquire data from IPC_RING_Buffer
        float *data = (float *)RingBuffer_acquired(receiver->m_buffer,size);
        if (data == NULL)
        {
                LOG_FATAL("Failed to acquire data");
        }

        if(wifisample.front == 0 && wifisample.rear == 0)
        {
                //Copy data
            memcpy(samples, data, size);

                for(i=0;i<1024;i++)
                {
                        wifisample_rx_enqueue(samples[i]);
                }
        }

        RingIO_release(receiver->m_buffer->m_read_handle, size);
        msg_free(msg);
    }

    receiver->m_running = 0;
    LOG_INFO("SampleReceiver is stopped");

    return (void *)0;
}

//Sending data to iPhone
//Wifi_sending_thread
void *Wifi_sending(void *arg)
{
    LOG_INFO("Wifi_sending starts running");

    struct timeval startTime,endTime;
    float Timeuse;

    char udpdata[1024*10];   //used for UDP sending data buffer
    char s[10];              //used for float to char converting temporary buffer
    int i,j;

    SampleReceiver *receiver = (SampleReceiver *)arg;

    while (receiver->m_running)
    {
    	gettimeofday(&startTime,NULL);
        if(wifisample.rear == 1024)
        {
            for(i=0;i<1024;i++)
            {
              gcvt(wifisample_rx_dequeue(),10,s);
              for(j=0;j<10;j++)
              {
                    udpdata[i*10+j]=s[j];
              }
            }

            memset(wifisample.sample_buff,0,1024);
            wifisample.front = 0;
            wifisample.rear  = 0;

            Udp_Send(receiver->m_udp,udpdata);

            gettimeofday(&endTime,NULL);
            Timeuse = 1000000*(endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec);
            Timeuse /= 1000000;

    //        LOG_INFO("Wifi_sending Time Use = %f\n",Timeuse);
        }
    }

    LOG_INFO("Wifi_sending is stopped");

    return (void *)0;
}



