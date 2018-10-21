#include <stdlib.h>
#include <string.h>

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Task.h>

#include "../dsp/app/ad_workshop.h"
#include "../shared/protocol.h"
#include "../dsp/app/queue.h"
#include "../dsp/app/message_loop.h"

static Void algorism_task_func(UArg arg0, UArg arg1);

//Algorism Select
extern int algorism_number;

//FIR Coefficients
extern float H[];

typedef struct _Raw
{
    Queue_Elem parent;
    float *data;                  //Point to the data in IPC RingIO
    unsigned int size;         //Data size
} Raw;    

//Define structure for AD data processing
struct _ADWorkshop
{
    Queue *queue;
    RingBuffer *buffer;
    unsigned int heap_id;
    MessageQ_QueueId notify_queue;
    int do_algorism;
    Task_Handle algorism_task;
    int algorism_task_running;
    int algorism_task_quit;
};

ADWorkshop *ad_workshop_new(RingBuffer *buffer, 
                            MessageQ_QueueId notify_queue, 
                            unsigned int heap_id)
{
    ADWorkshop *shop = (ADWorkshop *)malloc(sizeof(ADWorkshop));
    memset(shop, 0, sizeof(ADWorkshop));
    shop->do_algorism = 0;
    shop->buffer = buffer;
    shop->heap_id = heap_id;
    shop->queue = queue_new();
    shop->notify_queue = notify_queue;
    shop->algorism_task_running = 0;
    shop->algorism_task_quit = 0;

    //Create a task for algorism
	Error_Block eb;
	Error_init(&eb);
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.instance->name = "algorism";
	taskParams.priority = 13;
    taskParams.arg0 = (UArg)shop;
    taskParams.arg1 = (UArg)shop;
    taskParams.stackSize = 0x1000;

    // Start algorism thread
    shop->algorism_task = Task_create(algorism_task_func, &taskParams, &eb);
    if (shop->algorism_task == NULL)
        {
          Log_error0("Fail to creat algorism_task_func");
        }

    return shop;
}


 //Get the data from the queue to perform algorism
static Void algorism_task_func(UArg arg0, UArg arg1)
{
    Log_info0("algorism_task_func starts running");

    ADWorkshop *shop = (ADWorkshop *)arg0;

    shop->algorism_task_running = 1;
    while(! shop->algorism_task_quit)
    {
        Raw *raw = (Raw *)queue_pop(shop->queue);

		void FFT(float *Input, float *Cmo, unsigned int Tn);
		void FIR(float *rawdata, float *firout, float *H);

        /* Perform algorism */
    	switch(algorism_number)
    	{
    	   case 1:
    		   FIR(raw->data, raw->data, H);
			   free(raw);
    		   break;

    	   case 2:
			   FFT(raw->data, raw->data, raw->size);
			   free(raw);
    		   break;

    	   default:
    		   break;
    	}

        /* Send message to HOST */
        Message *msg = (Message *)MessageQ_alloc(shop->heap_id, MSG_SIZE);
        if (msg == NULL)
            {
               Log_error0("Fail to malloc message from heap");
            }

        msg->cmd = MSG_CMD_ALGORISM_DATA;
        MessageQ_put(shop->notify_queue, (MessageQ_Msg)msg);
    }

    shop->algorism_task_running = 0;
    Log_info0("algorism_task_func quits");
}


void ad_workshop_destroy(ADWorkshop *shop)
{
    if (! shop)
        return;

    /* Wait for algorism thread to quit */
    if (shop->algorism_task_running) {
        shop->algorism_task_quit = 1;
        while(! shop->algorism_task_running) {}
    }

    queue_destroy(shop->queue);
    free(shop);
}

void ad_workshop_do_algorism(ADWorkshop *shop, int flag)
{
    shop->do_algorism = flag;
}

//Put the data to algorism queue, and sent message to HOST
void ad_workshop_import(ADWorkshop *shop, float *data, unsigned int size)
{
    if (shop->do_algorism)
    {
        Raw *raw = (Raw *)malloc(sizeof(Raw));
        raw->data = data;
        raw->size = size;
        queue_push(shop->queue, (Queue_Elem *)raw);
    }
    else
    {
        Message *msg = (Message *)MessageQ_alloc(shop->heap_id, MSG_SIZE);
        if (msg == NULL)
            {
               Log_error0("Fail to malloc message from heap");
            }

        msg->cmd = MSG_CMD_RAW_DATA;
        MessageQ_put(shop->notify_queue, (MessageQ_Msg)msg);

    }
}


