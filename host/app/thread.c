#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include "../app/thread.h"
#include "../host/app/log.h"


Thread *Thread_new(char *thread_name, void *fun, int priority,pthread_t id, void *args)
{
    Thread *thread = (Thread *)malloc(sizeof(Thread));
    memset(thread, 0, sizeof(Thread));

    //Initialization
    thread->m_tid  = id;
    thread->m_name = thread_name ;
    thread->thread_fun = fun;
    thread->m_is_started = 0;
    thread->m_is_joined = 0;
    thread->m_priority = priority;
    thread->arg = args;

    return thread;
}

void  Thread_destroy(Thread *thread)
{
    if (thread->m_is_started && ! thread->m_is_joined)
    {
    	//Detach thread m_tid
    	//Wait for another thread to perform PTHREAD_JOIN
        pthread_detach(thread->m_tid);
    }
}

void set_priority(pthread_attr_t *tattr, Thread *thread)
{
    //Set thread attribution to default
    //This function should be called before pthread_create
    pthread_attr_init(tattr);

    //Set the thread priority policy
    pthread_attr_setschedpolicy(tattr, SCHED_FIFO);

    //Set the thread priority
    //Usually get the priority first then set that
    struct sched_param param;
    pthread_attr_getschedparam(tattr, &param);
    param.sched_priority = thread->m_priority;

    pthread_attr_setschedparam(tattr, &param);
}


void thread_start(Thread *thread)
{
    int temp;

    //Set the thread priority
    pthread_attr_t tattr;
    set_priority(&tattr, thread);

    //Create the thread
    if((temp=pthread_create(&thread->m_tid, &tattr, thread->thread_fun, (void *)thread->arg)) != 0)
      {
        LOG_INFO("%s thread is not created", thread->m_name);
      }
    else
      {
        LOG_INFO("%s thread is created", thread->m_name);
      }
    thread->m_is_started = 1;
}

void thread_join(Thread *thread)
{
    //Wait for thread m_tid to be finished
    pthread_join(thread->m_tid, NULL);
    thread->m_is_joined = 1;
}

pthread_t thread_id()
{
    //Obtain the identifier of the current thread
    return pthread_self();
}

int thread_is_runing_in(Thread *thread)
{
    //Compare two thread identifiers
    return pthread_equal(thread->m_tid, thread_id()) != 0;
}


