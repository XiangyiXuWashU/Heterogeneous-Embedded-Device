/* host header files */
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

/* package header files */
#include <ti/syslink/Std.h>     /* must be first */
#include <ti/syslink/IpcHost.h>
#include <ti/syslink/SysLink.h>
#include <ti/syslink/RingIO.h>
#include <ti/syslink/RingIOShm.h>
#include <ti/syslink/ProcMgr.h>
#include <ti/syslink/utils/IHeap.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/ipc/HeapBufMP.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/SharedRegion.h>

#include <pthread.h>

/* local header files */
#include "../shared/protocol.h"
#include "../ipc/message_queue.h"
#include "../ipc/message_heap.h"
#include "../ipc/ring_buffer_host.h"
#include "../ipc/message_queue.h"
#include "../network/wifi.h"
#include "../app/phone_command.h"
#include "../app/sample_receiver.h"
#include "../app/controler.h"
#include "../app/thread.h"
#include "../app/app.h"
#include "../host/app/log.h"

AppModule Module;

void App_create(UInt16 remoteProcId)
{
    pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t)*4);
    memset(tid,0,(sizeof(pthread_t)*4));

    //Initialize the App_Module
    Module.remoteProcId   = remoteProcId;
    Module.is_started     = 0;
    Module.m_buffer       = RingBuffer_new();
    Module.m_queue        = message_queue_new();

    //wifi_receiver_thread
    Module.m_udp                     = Udp_new();
    Module.m_phonecommand            = PhoneCommand_new();
    Module.paras                     = UdpReceiveParas_new(Module.m_udp,Module.m_phonecommand);
    Module.m_thread_wifireceive      = Thread_new("wifi_receiver_thread", Udp_Receive,50,tid[0],Module.paras);

    //sample_receiver_thread
    Module.m_receiver      = SampleReceiver_new(Module.m_buffer, Module.m_queue, Module.m_udp);
    Module.m_thread_sample = Thread_new("sample_receiver_thread", SampleReceiver_run,30,tid[1],Module.m_receiver);

    //Wifi_sending_thread
    Module.m_thread_wifisending = Thread_new("Wifi_sending_thread", Wifi_sending,30,tid[2],Module.m_receiver);

    //phone_command_process_thread
    Module.m_thread_phone = Thread_new("phone_command_process_thread", PhoneCommand_Process,20,tid[3],Module.m_phonecommand);

    Module.m_controler    = Controler_new(Module.m_queue);
}

void App_exec()
{
    Module.is_started  = 1;

    //Send a MSG_CMD_START messageQ to DSP
    Controler_send_cmd(Module.m_controler,MSG_CMD_START);

    //Send a MSG_CMD_ENABLE_ALGORISM messageQ to DSP
    Controler_send_cmd(Module.m_controler,MSG_CMD_ENABLE_ALGORISM);

    //Start receiving data from DSP and sending to iPhone from WIFI
    SampleReceiver_start(Module.m_receiver,Module.m_thread_sample);

    //Start the wifi_receiver_thread
    Udp_Receive_start(Module.paras, Module.m_thread_wifireceive);

    //Start the wifi_sending_thread
    Udp_Send_start(Module.paras, Module.m_thread_wifisending);

    //Start the phone_command_process_thread
    PhoneCommand_start(Module.m_phonecommand,Module.m_thread_phone);

    thread_join(Module.m_thread_wifireceive);
    thread_join(Module.m_thread_sample);
    thread_join(Module.m_thread_wifisending);
    thread_join(Module.m_thread_phone);
}


void App_delete()
{
    message_queue_destroy(Module.m_queue);
    RingBuffer_destroy(Module.m_buffer);
    SampleReceiver_destroy(Module.m_receiver,Module.m_thread_sample);
    Udp_destroy(Module.m_udp,Module.m_thread_wifireceive);
    Wifisending_destroy(Module.paras, Module.m_thread_wifisending);
    PhoneCommand_destroy(Module.m_phonecommand,Module.m_thread_phone);

    Module.is_started  = 0;
}


