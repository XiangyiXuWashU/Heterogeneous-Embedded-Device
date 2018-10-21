#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

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

#include "../app/controler.h"
#include "../app/sample_receiver.h"
#include "../app/thread.h"
#include "../app/phone_command.h"

#include "../ipc/ring_buffer_host.h"
#include "../ipc/message_queue.h"
#include "../network/wifi.h"
#include "../shared/protocol.h"
#include "../host/app/log.h"
#include "../app/app.h"

extern AppModule Module;

PhoneCommand *PhoneCommand_new()
{
    PhoneCommand *phoneCommand = (PhoneCommand *)malloc(sizeof(PhoneCommand));
    memset(phoneCommand, 0, sizeof(PhoneCommand));

    phoneCommand->m_running = 0;
    phoneCommand->startProcessWifiCommondData = 0;
    phoneCommand->startProcessWifiCommondDataFlag = 0;
    phoneCommand->IPHONE_COMMOND_BUF_INDEX = 0;
    phoneCommand->front = 0;
    phoneCommand->rear = 0;
    memset(phoneCommand->uart_buff,0,BUFF_MAX);

    return phoneCommand;
}


void  PhoneCommand_destroy(PhoneCommand *phoneCommand,Thread *m_thread)
{
    phoneCommand->m_running = 0;

    //Detach current thread and Wait for another thread to perform PTHREAD_JOIN
    Thread_destroy(m_thread);
}

//Push into queue
char PhoneCommand_rx_enqueue(PhoneCommand *phoneCommand,char x)
{
 if(phoneCommand->front == (phoneCommand->rear+1)%BUFF_MAX)
    return 0;
 else
    {
     phoneCommand->uart_buff[phoneCommand->rear] = x;
     phoneCommand->rear = (phoneCommand->rear+1)%BUFF_MAX;
     return 0;
    }
}

//Pop from queue
char PhoneCommand_rx_dequeue(PhoneCommand *phoneCommand)
{
     char temp;

     if(phoneCommand->front == phoneCommand->rear)
        return 0;
     else
     {
        temp = phoneCommand->uart_buff[phoneCommand->front];
        phoneCommand->front = (phoneCommand->front+1)%BUFF_MAX;
        return temp;
     }
}

void PhoneCommand_start(PhoneCommand *phoneCommand,Thread *m_thread)
{
    phoneCommand->m_running = 1;
    thread_start(m_thread);
}


void *PhoneCommand_Process(void *arg)
{
    LOG_INFO("PhoneCommand_Process starts running");

    int tempValue;

	PhoneCommand *phoneCommand = (PhoneCommand *)arg;

	while(phoneCommand->m_running)
	{
	  int action = 0;
	  char tempData;

	  if(phoneCommand->front != phoneCommand->rear)
	   {
                tempData = PhoneCommand_rx_dequeue(phoneCommand);
                phoneCommand->IPHONE_COMMOND_BUF[phoneCommand->IPHONE_COMMOND_BUF_INDEX++] = tempData;

                if(tempData==0x4C) phoneCommand->startProcessWifiCommondDataFlag=1;        //Judge L
                else if(tempData==0x59&&phoneCommand->startProcessWifiCommondDataFlag==1)  //Judge Y
                {
                    phoneCommand->startProcessWifiCommondData=1;
                    phoneCommand->IPHONE_COMMOND_BUF_INDEX=0;
                }
                else
                {
                    phoneCommand->startProcessWifiCommondDataFlag=0;
                    phoneCommand->startProcessWifiCommondData=0;
                }

           }

		if(phoneCommand->startProcessWifiCommondData)
		{
			if      (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"ENALGO"))     {action = 1;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"DISALGO"))    {action = 2;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"CHA"))        {action = 3;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"CHB"))        {action = 4;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"FIR"))        {action = 5;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"FFT"))        {action = 6;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEA"))      {action = 7;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEB"))      {action = 8;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEC"))      {action = 9;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATED"))      {action = 10;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEE"))      {action = 11;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEF"))      {action = 12;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEG"))      {action = 13;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEH"))      {action = 14;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEI"))      {action = 15;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEJ"))      {action = 16;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEK"))      {action = 17;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEL"))      {action = 18;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"RATEM"))      {action = 19;}

			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"H0:"))        {action = 25;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"H1:"))        {action = 26;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"H2:"))        {action = 27;}
			else if (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"H3:"))        {action = 28;}

			else
				{
					action =0;
				}

				switch(action)
				 {
						 case(1):
							//Send a MSG_CMD_ENABLE_ALGORISM to DSP
	                    	Controler_send_cmd(Module.m_controler,MSG_CMD_ENABLE_ALGORISM);
						 break;

						 case(2):
							//Send a MSG_CMD_DISABLE_ALGORISM to DSP
							Controler_send_cmd(Module.m_controler,MSG_CMD_DISABLE_ALGORISM);
						 break;

						 case(3):
							//Send a CHA messageQ to DSP
							Controler_send_cmd(Module.m_controler,MSG_CMD_CHA);
						 break;

						 case(4):
							//Send a CHB messageQ to DSP
							Controler_send_cmd(Module.m_controler,MSG_CMD_CHB);
						 break;

						 case(5):
							//Send a ALGORISM1 messageQ to DSP
							Controler_send_cmd(Module.m_controler,MSG_CMD_ALGORISM1);
						 break;

						 case(6):
							//Send a ALGORISM2 messageQ to DSP
							Controler_send_cmd(Module.m_controler,MSG_CMD_ALGORISM2);
						 break;

						 case(7):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE1);
						 break;

						 case(8):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE2);
						 break;

						 case(9):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE3);
						 break;

						 case(10):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE4);
						 break;

						 case(11):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE5);
						 break;

						 case(12):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE6);
						 break;

						 case(13):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE7);
						 break;

						 case(14):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE8);
						 break;

						 case(15):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE9);
						 break;

						 case(16):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE10);
						 break;

						 case(17):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE11);
						 break;

						 case(18):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE12);
						 break;

						 case(19):
							Controler_send_cmd(Module.m_controler,MSG_CMD_RATE13);
						 break;


						 //Sent FIR Coefficients H to DSP
						 //Format: H[0]="1"+"H0*100"
						 //Format: H[1]="2"+"H1*100"
						 //Format: H[2]="3"+"H2*100"
						 //Format: H[3]="4"+"H3*100"

						 case(25):
                    		tempValue = strtod (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"H0:")+3,NULL);
							Controler_send_cmd(Module.m_controler,tempValue+1000);
						 break;

						 case(26):
                    		tempValue = strtod (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"H1:")+3,NULL);
							Controler_send_cmd(Module.m_controler,tempValue+2000);
						 break;

						 case(27):
                    		tempValue = strtod (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"H2:")+3,NULL);
							Controler_send_cmd(Module.m_controler,tempValue+3000);
						 break;

						 case(28):
                    		tempValue = strtod (strstr((const char*)phoneCommand->IPHONE_COMMOND_BUF,"H3:")+3,NULL);
							Controler_send_cmd(Module.m_controler,tempValue+4000);
						 break;

				default:

						 break;
				 }
				phoneCommand->startProcessWifiCommondData=0;
				memset(phoneCommand->IPHONE_COMMOND_BUF,0,256);
			 }
	}

    return (void *)0;
}
