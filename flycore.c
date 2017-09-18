#include "flycore.h"
#include <sys/time.h>
#include <sys/select.h>
#include <time.h>
#include <stdio.h>

/*seconds: the seconds; mseconds: the micro seconds*/
void flymsleep(unsigned long msec){
    struct timeval tv;
    tv.tv_sec=msec/1000;
    tv.tv_usec=(msec%1000)*1000;
    int err;
    do{
       err=select(0,NULL,NULL,NULL,&tv);
    }while(err<0 && errno==EINTR);
}

void flyusleep(unsigned long usec){
    struct timeval tv;
    tv.tv_sec=usec/1000000;
    tv.tv_usec=usec%1000000;
    int err;
    do{
        err=select(0,NULL,NULL,NULL,&tv);
    }while(err<0 && errno==EINTR);
}

void flysleep(unsigned long sec,unsigned long usec)
{
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec= usec;
    int err;

    do{
        err=select(0,NULL,NULL,NULL,&tv);
    }while(err<0 && errno==EINTR);
}
