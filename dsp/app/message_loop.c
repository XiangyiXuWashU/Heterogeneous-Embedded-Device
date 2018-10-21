#include <stdlib.h>
#include <string.h>

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>

#include <ti/ipc/MessageQ.h>

#include "../dsp/ipc/message_heap.h"
#include "../dsp/app/ad_workshop.h"
#include "../shared/protocol.h"
#include "../dsp/driver/ring_buffer.h"
#include "../dsp/app/message_loop.h"
#include "../dsp/ipc/ipc_ring_buffer.h"
#include "../dsp/driver/setgpio.h"

//FIR Coefficients
extern float H[];

struct _MessageLoop
{
    MessageQ_Handle queue;
    int quit;
    int is_started;

    ad9238 *ad;                           //AD9238 Module
    ADWorkshop *shop;                //AD Signal Processing
    MessageHeap *heap;              //Heap for MessageQ
    RingBuffer *buffer;                 //Shared Data Buffer
    MessageQ_QueueId reply_id;  //Reply ID for MessageQ
};

//Function Declaration
static void message_loop_on_ad_trigger(float *data, void *user_data);

//Algorism Select
int algorism_number = 1;

//It will be called in message_loop_fun
MessageLoop *message_loop_new()
{
    MessageLoop *loop = (MessageLoop *)malloc(sizeof(MessageLoop)); 
    memset(loop, 0, sizeof(MessageLoop));

    MessageQ_Params params;
    MessageQ_Params_init(&params);
    loop->queue = MessageQ_create(MSG_QUEUE_NAME_SLAVE, &params);
    loop->heap = message_heap_new(SHARED_REGION_ID_MSG_QUEUE);

    loop->quit = 0;
    loop->is_started = 0;

    return loop;
}

//It will be called in message_loop_fun
void message_loop_destroy(MessageLoop *loop)
{
    if (! loop)
        return;

    if (loop->shop)
        ad_workshop_destroy(loop->shop);

    switchLED1(0);
    switchLED2(0);
    ad9238_destroy(loop->ad);
    MessageQ_delete(&loop->queue);
    message_heap_destroy(loop->heap);
    ring_buffer_destroy(loop->buffer);
    free(loop);
}

//Command processing for MSG_CMD_START command
static void message_loop_on_start(MessageLoop *loop, Message *msg)
{
    if (loop->is_started)
    {
    	Log_warning0("MSG_CMD_START has already been received ");
        return;
    }

    //Save MessageQ ID for HOST
    loop->reply_id = MessageQ_getReplyQueue(msg); 

    loop->buffer = ipc_ring_buffer_new(RING_BUFFER_NAME);
    loop->ad = ad9238_new(loop->buffer);
    loop->shop = ad_workshop_new(loop->buffer, loop->reply_id, message_heap_id(loop->heap));

    //Register Call-back Event
    //Register message_loop_on_ad_trigger to ad->callback
    //When uPP transfer is finished, the callback function will call message_loop_on_ad_trigger
    ad9238_trigger(loop->ad, message_loop_on_ad_trigger, loop);
    startuPPTask(loop->ad);

    loop->is_started = 1;
}

//message_loop_on_ad_trigger is callback function
//When the uPP is finished, it will call-back this function
//The parameters format should be same with TRIGGER_CALLBACK
//Update the user_data and ad_workshop_import data
static void message_loop_on_ad_trigger(float *data, void *user_data)
{
    MessageLoop *loop = (MessageLoop *)user_data;
    ad_workshop_import(loop->shop, data, 1024);
}


//Enable Algorism Processing for data
static void message_loop_on_enable_algorism(MessageLoop *loop, Message *msg)
{
    if (! loop->is_started)
    {
    	Log_warning0("MSG_CMD_START should be received first");
    }

    else 
    {
        ad_workshop_do_algorism(loop->shop, 1);
    }
}

//Disable Algorism Processing for data
static void message_loop_on_disable_algorism(MessageLoop *loop, Message *msg)
{
    if (! loop->is_started)
    {
    	Log_warning0("MSG_CMD_START should be received first");
    }

    else 
    {
        ad_workshop_do_algorism(loop->shop, 0);
    }
}

//All commands are processed in this function
//It will be called in message_loop_fun
void message_loop_start(MessageLoop *loop)
{
	Log_info0("message loop start");
    loop->quit = 0;

    while (! loop->quit)
    {
        //Wait command from HOST
       Message *msg;
       MessageQ_get(loop->queue, (MessageQ_Msg *)&msg, MessageQ_FOREVER); //Stop here waiting for message from HOST

        switch (msg->cmd) {
            case MSG_CMD_START:
                message_loop_on_start(loop, msg);
                break;

            case MSG_CMD_ENABLE_ALGORISM:
                message_loop_on_enable_algorism(loop, msg);
                break;

            case MSG_CMD_DISABLE_ALGORISM:
                message_loop_on_disable_algorism(loop, msg);
                break;

            case MSG_CMD_CHA:
                selectADCChannel(0);
                break;

            case MSG_CMD_CHB:
                selectADCChannel(1);
                break;

            case MSG_CMD_ALGORISM1:
            	algorism_number = 1;   //FIR Algorism
                break;

            case MSG_CMD_ALGORISM2:
            	algorism_number = 2;   //FFT Algorism
                break;

            case MSG_CMD_RATE1:
                sampleRateControl(0, 0, 0, 0);//Real_ADC_CLK = 10KHz
                break;

            case MSG_CMD_RATE2:
                sampleRateControl(0, 0, 0, 1);//Real_ADC_CLK = 20KHz
                break;

            case MSG_CMD_RATE3:
                sampleRateControl(0, 0, 1, 0);//Real_ADC_CLK = 50KHz
                break;

            case MSG_CMD_RATE4:
                sampleRateControl(0, 0, 1, 1);//Real_ADC_CLK = 100KHz
                break;

            case MSG_CMD_RATE5:
                sampleRateControl(0, 1, 0, 0);//Real_ADC_CLK = 200KHz
                break;

            case MSG_CMD_RATE6:
                sampleRateControl(0, 1, 0, 1);//Real_ADC_CLK = 500KHz
                break;

            case MSG_CMD_RATE7:
                sampleRateControl(0, 1, 1, 0);//Real_ADC_CLK = 1MHz
                break;

            case MSG_CMD_RATE8:
                sampleRateControl(0, 1, 1, 1);//Real_ADC_CLK = 2MHz
                break;

            case MSG_CMD_RATE9:
                sampleRateControl(1, 0, 0, 0);//Real_ADC_CLK = 5MHz
                break;

            case MSG_CMD_RATE10:
                sampleRateControl(1, 0, 0, 1);//Real_ADC_CLK = 10MHz
                break;

            case MSG_CMD_RATE11:
                sampleRateControl(1, 0, 1, 0);//Real_ADC_CLK = 20MHz
                break;

            case MSG_CMD_RATE12:
                sampleRateControl(1, 0, 1, 1);//Real_ADC_CLK = 25MHz
                break;

            case MSG_CMD_RATE13:
                sampleRateControl(1, 1, 0, 0);//Real_ADC_CLK = 50MHz
                break;


            default:

                if(msg->cmd>=1000&&msg->cmd<=5000)
                {
                	if((msg->cmd/1000) == 1)
                	{
                		H[0] = ((float)(msg->cmd%1000))/100;
                	}
                	else if((msg->cmd/1000) == 2)
                	{
                		H[1] = ((float)(msg->cmd%1000))/100;
                	}
                	else if((msg->cmd/1000) == 3)
                	{
                		H[2] = ((float)(msg->cmd%1000))/100;
                	}
                	else if((msg->cmd/1000) == 4)
                	{
                		H[3] = ((float)(msg->cmd%1000))/100;
                	}
                }

                break;
        }

        MessageQ_free((MessageQ_Msg)msg);
    }

    loop->quit = 1;

	Log_info0("message loop end");

}


void message_loop_stop(MessageLoop *loop)
{
    loop->quit = 1;
}
