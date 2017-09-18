/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    throughput_test1.c
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-07-31 7:15:00
 * updated: 2017-07-31 7:15:00
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#include "libipc.h"

struct timeval start, stop;

void my_mq_recv(struct msg_st *msg)
{
    int type = 0;
    int64_t delta = 0;
    gettimeofday(&stop, NULL);
    GET_DATA_TYPE(type,msg->msg_type);
    if(type == DATA_TYPE_BINARY){
        if(IS_DEFINED(msg->msg_type,MSG_IPC_SHM)){
            printf("===============================================================\n");
            int i = 0;
            long size = msg->msg_size;
            unsigned char *pchar = (unsigned char *)msg->msg_shmdata;
            if(pchar == NULL) return;
            //delta = ((stop.tv_sec - start.tv_sec) * 1000000000 + stop.tv_usec - start.tv_usec) * 1000;
            delta = stop.tv_usec - start.tv_usec;
            #if 0
            for(i = 0; i< size; i++){
                printf("%2X ",*(pchar + i));
                if(i % 20 == 0 && i != 0)
                    printf("\n");
            }
            #endif
            printf("\nReceived a shm : %s  size: %ld latency %li\n",msg->msg_shmid,msg->msg_data._long,delta );
            printf("===============================================================\n");
            if(*(pchar +1) == 0){
                exit(0);
            }

            return;
        }
    }
}

char *generate_huge_data(long size)
{
    char *tmp = NULL;
    int i;
    unsigned char count = 0;
    static unsigned char sep =0;
    tmp = (char *)malloc(size);
    if(tmp != NULL)
    {
        for(i = 0; i < size -1; i++){
            tmp[i] = count++;
        }
    }
    
    tmp[size -1] = sep++;

    return tmp;
}

int main(int argc, char **argv)
{
    int ret = -1, loop = 1;

    ret = msg_init("T1",0,my_mq_recv);
    while(loop){
        char *data = generate_huge_data(256*10000);
        msg_st *msg = msg_create_binary("binary",data,256*10000);
        gettimeofday(&start, NULL);
        msg_send_free("T2",&msg,10);
        sleep(1);
    }
    
    return ret;
}