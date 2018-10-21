#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <ti/syslink/Std.h>     /* must be first */

#include "../network/wifi.h"
#include "../app/phone_command.h"
#include "../shared/protocol.h"
#include "../host/app/log.h"

unsigned char startReadWifiCommondData =0;
unsigned char startReadWifiCommondDataFlag =0;
unsigned char stopReadWifiCommondDataFlag =0;


void info(char *s)
{
    perror(s);
    exit(1);
}

Udp *Udp_new()
{
    Udp *udp = (Udp *)malloc(sizeof(Udp));
    memset(udp, 0, sizeof(Udp));

    udp->m_receive_running = 0;
    udp->m_send_running = 0;

    //Set the Receiving socket
    //create a UDP socket
    if ((udp->s_receive=socket(AF_INET, SOCK_DGRAM, PF_UNSPEC)) == -1)
    {
    	info("Receiving socket");
    }

    //zero out the structure
    memset((char *) &udp->receive_addr, 0, sizeof(udp->receive_addr));

    udp->receive_addr.sin_family = AF_INET;
    udp->receive_addr.sin_port = htons(8086);
    udp->receive_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to local server port
    if(bind(udp->s_receive, (struct sockaddr*)&udp->receive_addr, sizeof(udp->receive_addr) ) == -1)
    {
        info("Receiving socket bind");
    }

    //Set the Sending socket
    //create a UDP socket
    if ((udp->s_send=socket(AF_INET, SOCK_DGRAM, PF_UNSPEC)) == -1)
    {
    	info("Sending socket");
    }

    //zero out the structure
    memset((char *) &udp->send_addr, 0, sizeof(udp->send_addr));

    udp->send_addr.sin_family = AF_INET;
    udp->send_addr.sin_port = htons(8088);
    udp->send_addr.sin_addr.s_addr = inet_addr("192.168.3.2");
    return udp;
}


void  Udp_destroy(Udp *udp,Thread *udp_thread)
{
    udp->m_receive_running = 0;

    //Detach current thread and Wait for another thread to perform PTHREAD_JOIN
    Thread_destroy(udp_thread);

    close(udp->s_receive);
    close(udp->s_send);
}


//Send Data to WIFI
void Udp_Send(Udp *udp,char *newdata)
{
    //Update the send_buff
    memset(udp->send_buff, 0, SEND_BUF_SIZE);

    memcpy(udp->send_buff, newdata, SEND_BUF_SIZE);

    //send the data
    if (sendto(udp->s_send, udp->send_buff, sizeof(udp->send_buff) , 0 ,
                    (struct sockaddr *) &udp->send_addr, sizeof(udp->send_addr))==-1)
    {
       info("sendto()");
    }
}

UdpReceiveParas *UdpReceiveParas_new(Udp *udp,PhoneCommand *phoneCommand)
{
    UdpReceiveParas *paras = (UdpReceiveParas *)malloc(sizeof(UdpReceiveParas));
    memset(paras, 0, sizeof(UdpReceiveParas));

    paras->udp = udp;
    paras->phoneCommand = phoneCommand;

    return paras;
}

void Udp_Receive_start(UdpReceiveParas *paras, Thread *thread)
{
    paras->udp->m_receive_running = 1;
    thread_start(thread);
}

void Udp_Send_start(UdpReceiveParas *paras, Thread *thread)
{
    paras->udp->m_send_running = 1;
    thread_start(thread);
}

void  Wifisending_destroy(UdpReceiveParas *paras, Thread *thread)
{
    paras->udp->m_send_running = 0;

    //Detach current thread and Wait for another thread to perform PTHREAD_JOIN
    Thread_destroy(thread);
}


//Receive Data from WIFI
void *Udp_Receive(void *arg)
{
    int i;

    LOG_INFO("Udp_Receive starts running");

    UdpReceiveParas  *paras = (UdpReceiveParas *)arg;

    paras->udp->m_receive_running = 1;

    while(paras->udp->m_receive_running)
    {
        socklen_t len = sizeof(paras->udp->receive_addr);

        memset(paras->udp->receive_buff, 0, RECEIVE_BUF_SIZE);

        //Try to receive data, this is a blocking call
        if (recvfrom(paras->udp->s_receive, paras->udp->receive_buff, RECEIVE_BUF_SIZE, 0,
                        (struct sockaddr *) &(paras->udp->receive_addr), &len) == -1)
        {
            info("recvfrom()");
        }

        else
        {
            //Insert the receiving command to the queue
            for(i=0;i < RECEIVE_BUF_SIZE;i++)
            {
                judgeIsIphoneSendCommond(paras, paras->udp->receive_buff[i]);
            }
        }
	}

    return (void *)0;
}


//Select iPhone Command started with XZ and ended with LY
void judgeIsIphoneSendCommond(UdpReceiveParas  *paras, char inputData)
{
    if(inputData == ALPHABET_X)
    {
     startReadWifiCommondDataFlag=1; //Judge X
    }
    else if((inputData == ALPHABET_Z)&&(startReadWifiCommondDataFlag == 1))//Judge Z
    {
     startReadWifiCommondData = 1;
     PhoneCommand_rx_enqueue(paras->phoneCommand,ALPHABET_X);
    }
    else
    {
      startReadWifiCommondDataFlag=0;
    }

    if(inputData == ALPHABET_L)
    {
        stopReadWifiCommondDataFlag = 1;//Judge L
    }
    else if((inputData == ALPHABET_Y)&&(stopReadWifiCommondDataFlag == 1))//Judge Y
    {
      startReadWifiCommondData = 0;
      PhoneCommand_rx_enqueue(paras->phoneCommand,inputData);
    }
    else
    {
      stopReadWifiCommondDataFlag=0;
    }

    if(startReadWifiCommondData)
    {
      PhoneCommand_rx_enqueue(paras->phoneCommand,inputData);
    }
}
