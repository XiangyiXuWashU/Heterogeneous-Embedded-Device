#ifndef THREAD_H_
#define THREAD_H_

#if defined __cplusplus
extern "C" {
#endif

struct _Thread
{
    pthread_t m_tid;     //Thread ID
    char *m_name;        //Thread name
    void *thread_fun;    //Thread function
    int m_is_started;    //Thread started flag
    int m_is_joined;     //Thread joined flag
    int m_priority;      //Thread priority flag
    void *arg;           //Thread parameters
};

typedef struct _Thread Thread;

Thread *Thread_new(char *thread_name, void *fun, int priority,pthread_t id,void *args);
void thread_start(Thread *thread);
void thread_join(Thread *thread);
void  Thread_destroy(Thread *thread);
void thread_set_priority(pthread_attr_t *tattr, Thread *thread);
pthread_t thread_id();
int thread_is_runing_in(Thread *thread);

#if defined __cplusplus
}
#endif

#endif
