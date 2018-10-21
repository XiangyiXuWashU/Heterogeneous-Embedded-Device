#ifndef WIFI_H_
#define WIFI_H_

#include <netinet/in.h>
#include "../app/thread.h"
#include "../app/phone_command.h"

#if defined __cplusplus
extern "C" {
#endif

#define SEND_BUF_SIZE 1024*10
#define RECEIVE_BUF_SIZE 256

#define  ALPHABET_X      0x58
#define  ALPHABET_Z      0x5A
#define  ALPHABET_L      0x4C
#define  ALPHABET_Y      0x59

struct _Udp
{
    int s_receive;                        //Receive socket describer
    int s_send;                           //send socket describer
    volatile int m_receive_running;       //Udp_Receive running flag
    volatile int m_send_running;          //Udp_Send running flag
    struct sockaddr_in receive_addr;      //Receive Socket address
    struct sockaddr_in send_addr;         //send Socket address
    char  send_buff[SEND_BUF_SIZE];       //Data sending buffer
    char  receive_buff[RECEIVE_BUF_SIZE]; //Data receiving buffer
};
typedef struct _Udp Udp;

struct _UdpReceiveParas
{
    Udp *udp;
    PhoneCommand *phoneCommand;
};
typedef struct _UdpReceiveParas UdpReceiveParas;

Udp  *Udp_new();
void  Udp_destroy(Udp *udp,Thread *udp_thread);
void  Wifisending_destroy(UdpReceiveParas *paras, Thread *thread);
void  Udp_Send(Udp *udp,char *newdata);
UdpReceiveParas *UdpReceiveParas_new(Udp *udp,PhoneCommand *phoneCommand);
void  Udp_Receive_start(UdpReceiveParas *paras, Thread *thread);
void  Udp_Send_start(UdpReceiveParas *paras, Thread *thread);
void  *Udp_Receive(void *arg);
void  judgeIsIphoneSendCommond(UdpReceiveParas  *paras, char inputData);


#if defined __cplusplus
}
#endif

#endif
