#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../dsp/driver/ad9238.h"

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/family/c64p/Cache.h>
#include <math.h>


#include "hw/hw_types.h"                          //Macros
#include "hw/hw_syscfg0_OMAPL138.h"       //Hardware definitions for OMAPL138
#include "hw/hw_psc_OMAPL138.h"            //Hardware definitions for OMAPL138
#include "hw/soc_OMAPL138.h"                 //peripheral information for OMAPL138 SOC
#include "c674x/omapl138/interrupt.h"       //DSP interrupt related API
#include "hw_upp.h"                               //uPP register definitions
#include "upp.h"                                    //Abstraction layer for uPP
#include "../dsp/app/message_loop.h"
#include "../dsp/app/ad_workshop.h"
#include "uPP_Config.h"
#include "../dsp/ipc/ipc_ring_buffer.h"
#include "../dsp/driver/setgpio.h"

//uPP internal DMA Configuration
#define upp_line_size         (1024)
#define upp_line_count       (1)
#define upp_frame_size      (upp_line_size * upp_line_count)
#define upp_line_offset      (upp_line_size)

//Global variable
extern volatile int upp_interrupt_count;
extern volatile int upp_error_count;
extern int do_Algorism_Global;

//uPP Receive Data Buffer
#pragma DATA_ALIGN(upp_buffer_a, 16)
unsigned short upp_buffer_a[upp_frame_size];

//uPP DMA Configuration Variable
uPPDMAConfig transposeParA;

//Function Declaration
static Void uPP_task_func(UArg arg0, UArg arg1);

int frameNumber=1;

//Module for ad9238
struct _ad9238
{
    float *data;                              //ADC Data
    RingBuffer *buffer;                    //Ring Buffer
    int counter;                              //Data Count

    TRIGGER_CALLBACK callback;     //Call-back
    void *user_data;                      //Can point to any type of data

    int uPP_task_running;
    int uPP_task_quit;
};


ad9238 * ad9238_new(RingBuffer *buffer)
{
    ad9238 *ad = (ad9238 *)malloc(sizeof(ad9238));
    memset(ad, 0, sizeof(ad9238));

    ad->buffer = buffer;
    ad->counter = 0;
    ad->uPP_task_running = 0;
    ad->uPP_task_quit = 0;

    return ad;
}

void startuPPTask(ad9238 *ad)
{
    Task_Handle uPP_task;             //Define uPP Task

    //uPP Setup
	OmaplFpgauPPSetup();

	uPPIntRegister();

	//Create a uPP Task
	Error_Block eb;
	Error_init(&eb);
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.instance->name = "uPP";
	taskParams.priority = 12;
    taskParams.arg0 = (UArg)ad;
    taskParams.arg1 = (UArg)ad;
    taskParams.stackSize = 0x1000;


    /* Start uPP thread */
    uPP_task = Task_create(uPP_task_func, &taskParams, &eb);
    if (uPP_task == NULL)
        {
          Log_error0("Fail to create uPP_task_func");
        }

}

//uPP Task
static Void uPP_task_func(UArg arg0, UArg arg1)
{
	int i =0;
	ad9238 *ad = (ad9238 *)arg0;

	transposeParA.WindowAddress     = (unsigned int *)((int)upp_buffer_a);
	transposeParA.LineCount             = upp_line_count;
	transposeParA.ByteCount             = (upp_line_size*sizeof(unsigned short));
	transposeParA.LineOffsetAddress  = (upp_line_offset*sizeof(unsigned short));

	Log_info0("uPP_task_func starts");

    ad->uPP_task_running = 1;
    while(! ad->uPP_task_quit)
    {
        //Require Ring_Buffer
        if (ad->counter == 0)
        {
        	ad->data = (float *)ring_buffer_acquired(ad->buffer, upp_line_size* sizeof(float));

            if (ad->data == NULL)
            {
            	Log_warning0("Fail to require ring buffer");
                Task_sleep(1000);
            }
        }

    	//Transfer uPP data
    	memset(upp_buffer_a, 0, sizeof(upp_buffer_a));

    	upp_error_count = 0;
    	upp_interrupt_count = 0;

		Cache_wb((void *)upp_buffer_a, sizeof(upp_buffer_a), Cache_Type_ALLD, TRUE);

		uPPDMATransfer(SOC_UPP_0_REGS, uPP_DMA_CHI,  &transposeParA);

		Cache_inv((void *)upp_buffer_a, sizeof(upp_buffer_a), Cache_Type_ALLD, TRUE);

    	//Wait uPP Received finished
    	while (upp_interrupt_count < 1 && upp_error_count == 0);

    	//Copy the uPP buffer data and separate ADC channel data
    	if (ad->data != NULL)
    	{
    		int numberExtratPerframe = upp_line_size/frameNumber;

			for( i=0; i<numberExtratPerframe;i++)
			{
			  //Real Voltage (mV) = FPGA Read Value * 2 * 1000 / 4096
			  //First four bits of upp_buffer_a is  oeb_a(BIT3), pdwn_a(BIT2), otr_a(BIT1), 1'b0(BIT0)
			  //oeb_a is Output Enable Bit for Channel A
			  //pdwn_a is Power-Down Function Selection for Channel A
			  //otr_a is Out-of-Range Indicator for Channel A

			  //Real Voltage (mV) = FPGA Read Value * 2 * 1000 / 4096
			  //First four bits of upp_buffer_a is  oeb_b(BIT3), pdwn_b(BIT2), otr_b(BIT1), 1'b1(BIT0)
			  //oeb_b is Output Enable Bit for Channel B
			  //pdwn_b is Power-Down Function Selection for Channel B
			  //otr_b is Out-of-Range Indicator for Channel B

			  ad->data[ad->counter++] =((upp_buffer_a[i*frameNumber*upp_line_count]>>4)&0xFFF)*1000/2048;
			}

    	}

    	if (ad->counter == upp_line_size)
    	{
    		ad->counter = 0;
			//Release Ring_Buffer
			ring_buffer_release(ad->buffer, upp_line_size*sizeof(float));

			//It will callback message_loop_on_ad_trigger
			if (ad->callback)
				ad->callback(ad->data, ad->user_data);
    	}

		if(upp_error_count != 0)
		{
	    	Log_info0("uPP Received error");
		}

    }

    ad->uPP_task_running = 0;
	Log_info0("uPP_task_func quits");

}

void judgeADCoutofRange(unsigned short inputA, unsigned short inputB)
{
	int a, b;

	a = inputA>>1&0x1;
	b = inputB>>1&0x1;

	if(a==1) switchLED1(1);
	else switchLED1(0);

	if(b==1) switchLED2(1);
	else switchLED2(0);
}

void ad9238_trigger(ad9238 *ad, TRIGGER_CALLBACK cb, void *user_data)
{
	Log_info0("ad9238_trigger");
    ad->callback = cb;
    ad->user_data = user_data;
}

void ad9238_destroy(ad9238 *ad)
{
	Log_info0("ad9238_destroy");
    if (ad)
        free(ad);
    ad->uPP_task_quit = 1;
}




