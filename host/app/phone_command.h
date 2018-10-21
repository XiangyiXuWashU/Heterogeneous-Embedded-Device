#ifndef PHONE_COMMAND_H_
#define PHONE_COMMAND_H_

#include "../app/thread.h"

#define  BUFF_MAX  2048

#if defined __cplusplus
extern "C" {
#endif

struct _PhoneCommand
{
    volatile int m_running;                 //phone command thread running flag
    unsigned short	front;                  //Queue Front
    unsigned short	rear;	                //Queue rear
    unsigned char	uart_buff[BUFF_MAX];    //WIFI Receive Command from iPhone
    int startProcessWifiCommondData;        //Start process the command from phone
    int startProcessWifiCommondDataFlag;    //Used to judge the terminal symbol from phone
    char IPHONE_COMMOND_BUF[256];          //Used to store the command to be processed
    int IPHONE_COMMOND_BUF_INDEX;           //Index of IPHONE_COMMOND_BUF
};
typedef struct _PhoneCommand PhoneCommand;

PhoneCommand *PhoneCommand_new();
void PhoneCommand_destroy(PhoneCommand *phoneCommand,Thread *m_thread);
char PhoneCommand_rx_enqueue(PhoneCommand *phoneCommand,char x);
char PhoneCommand_rx_dequeue(PhoneCommand *phoneCommand);
void PhoneCommand_start(PhoneCommand *phoneCommand,Thread *m_thread);
void *PhoneCommand_Process(void *arg);


#if defined __cplusplus
}
#endif


#endif
