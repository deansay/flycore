#include <stdio.h>
#include <stdlib.h>

#include "timer.h"
#include "log.h"

flytimer_t *_timer_init(timer_sub sub)
{
    flytimer_t * timer_ctx = (flytimer_t *)calloc(1, sizeof(flytimer_t));

    if(timer_ctx == NULL)
        return NULL;

    timer_ctx->ft_status = TIMER_STOPED;
    timer_ctx->ft_sub = sub;
    if(pthread_mutex_init(&timer_ctx->ft_mutex,NULL) != 0)
        log_error("mutex init error");

    return timer_ctx;
}

int _timer_deinit(flytimer_t *timer)
{
    if(timer != NULL)
    {
        _timer_stop(timer);
        free(timer);
    }

    return 0;
}


int _timer_stop(flytimer_t *timer)
{
    pthread_mutex_lock(&timer->ft_mutex);
    if(timer->ft_status  == TIMER_STOP){
        pthread_mutex_unlock(&timer->ft_mutex);
        return 0;
    }

    timer->ft_status = TIMER_STOP;
    pthread_mutex_unlock(&timer->ft_mutex);
    pthread_cancel(timer->ft_thread);
    pthread_join(timer->ft_thread, NULL); //wait the thread stopped
    return 0;
}

int _timer_start(flytimer_t *timer, void *data)
{
    if(timer == NULL){
        log_error("flytimer is null.");
        return -1;
    }
    _timer_stop(timer);
    pthread_mutex_lock(&timer->ft_mutex);
    timer->ft_userdata = data;
    timer->ft_status = TIMER_RUNNING;
    pthread_mutex_unlock(&timer->ft_mutex);

    pthread_create(&timer->ft_thread, NULL, _timer_routine, timer);
    return 0;
}

int _timer_settime(flytimer_t *timer, long msec)
{
    timer->ft_msec = msec;
    return 0;
}

void *_timer_routine(void *timer)
{
     flytimer_t * timer_ctx = (flytimer_t *)timer;

     int slice = timer_ctx->ft_msec / TIMER_PRECISION;
     int count = 0;
     struct timeval tv;
     tv.tv_sec = 0;
     tv.tv_usec=TIMER_PRECISION *1000; //convert to millisecond.

     do{
         pthread_mutex_lock(&timer_ctx->ft_mutex);
         if(timer_ctx->ft_status != TIMER_RUNNING){
             pthread_mutex_unlock(&timer_ctx->ft_mutex);
             return NULL;
         }

        count ++;
        select(0,NULL,NULL,NULL,&tv);
        if(count == slice)
            break;

     }while(1);

     pthread_mutex_lock(&timer_ctx->ft_mutex);
     timer_ctx->ft_sub(timer_ctx->ft_userdata);
     timer_ctx->ft_status = TIMER_STOPED;
     timer_ctx->ft_thread = -1;
     pthread_mutex_unlock(&timer_ctx->ft_mutex);

     return NULL;
}
