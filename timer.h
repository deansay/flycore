#ifndef TIMER_H
#define TIMER_H

#include <pthread.h>
#include <sys/time.h>

#define TIMER_PRECISION 10

typedef void (*timer_sub)(void *);
typedef enum flytimer_status{
    TIMER_STOP,
    TIMER_STOPED,
    TIMER_RUNNING,
}timer_status;

typedef struct flytimer_t{
     pthread_t       ft_thread;
     timer_status    ft_status;
     void            *ft_userdata;
     long            ft_msec;
     timer_sub       ft_sub;
     pthread_mutex_t ft_mutex;
}flytimer_t;

flytimer_t *_timer_init(timer_sub sub);
int _timer_deinit(flytimer_t *timer);
int _timer_settime(flytimer_t *timer,long msec);
int _timer_stop(flytimer_t *timer);
int _timer_start(flytimer_t *timer,void *data);
void *_timer_routine(void *timer);
#endif // TIMER_H
