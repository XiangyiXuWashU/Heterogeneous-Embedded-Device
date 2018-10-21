#ifndef _APP_H_
#define _APP_H_

#include "../ipc/message_queue.h"
#include "../ipc/ring_buffer_host.h"
#include "../app/thread.h"
#include "../app/sample_receiver.h"
#include "../app/controler.h"

#if defined (__cplusplus)
extern "C" {
#endif

/* module structure */
struct _AppModule
{
    UInt16          remoteProcId;            //remoteProcId = MultiProc_getId("DSP")
    int             is_started;              //Application started flag
    MessageQueue    *m_queue;                //MessageQ between two processors
    HostRingBuffer  *m_buffer;               //RingIO for Data Buffer
    Thread          *m_thread_wifireceive;   //wifi_receiver_thread
    Thread          *m_thread_sample;        //sample_receiver_thread
    Thread          *m_thread_wifisending;   //Wifi_sending_thread
    Thread          *m_thread_phone;         //phone_command_process_thread
    SampleReceiver  *m_receiver;             //Receiver data from DSP and Send them out
    Udp             *m_udp;                  //WIFI UDP
    PhoneCommand    *m_phonecommand;         //Process iPhone command
    UdpReceiveParas *paras;                  //Combination of Udp and PhoneCommand
    Controler       *m_controler;            //Command processing
};
typedef struct _AppModule AppModule;


void App_create(UInt16 remoteProcId);
void App_exec();
void App_delete();


#if defined (__cplusplus)
}
#endif
#endif
