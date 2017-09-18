/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    main_server.c
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-08-02 16:29:41
 * updated: 2017-08-02 16:29:41
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#include "libipc.h"

void my_mq_recv(struct msg_st *msg)
{
 
    int type = 0;
    static int count = 0;
    char msg_buffer[128] = {};

    #if 0
    printf("msg->msg_type : %ld\n",msg->msg_type);
    printf("msg->msg_shmid : %s\n",msg->msg_shmid);
    printf("msg->msg_name : %s\n",msg->msg_name);
    printf("msg->msg_module : %s\n",msg->msg_module);
    printf("msg->msg_shmdata : %x\n",(unsigned int)(long)msg->msg_shmdata);
    #endif

    GET_DATA_TYPE(type,msg->msg_type);

    if(type == DATA_TYPE_INT){
        printf("Received a %s value: %d\n",msg->msg_name,msg->msg_data._int);
    }else if(type == DATA_TYPE_DOUBLE){
        printf("Received a %s value: %.20lf\n",msg->msg_name,msg->msg_data._double);
    }else if(type == DATA_TYPE_FLOAT){
        printf("Received a %s value  %f\n",msg->msg_name,msg->msg_data._float);
    }else if(type == DATA_TYPE_SHORT){
        printf("Received a %s value: %d\n",msg->msg_name,msg->msg_data._short);
    }else if(type == DATA_TYPE_LONG){
        printf("Received a %s value: %ld\n",msg->msg_name,msg->msg_data._long);
    }else if(type == DATA_TYPE_CHAR){
        printf("Received a %s value: %c\n",msg->msg_name,msg->msg_data._char);
    }else if(type == DATA_TYPE_UINT){
        printf("Received a %s value: %u\n",msg->msg_name,msg->msg_data._uint);
    }else if(type == DATA_TYPE_USHORT){
        printf("Received a %s value: %u\n",msg->msg_name,msg->msg_data._ushort);
    }else if(type == DATA_TYPE_UCHAR){
        printf("Received a %s value: %u\n",msg->msg_name,msg->msg_data._uchar);
    }else if(type == DATA_TYPE_STRING){
        printf("Received a %s value: %s\n",msg->msg_name,msg->msg_data._buffer);
    }else if(type == DATA_TYPE_JSON){
        printf("Received a %s value: %s\n",msg->msg_name,msg->msg_data._buffer);
    }else if(type == DATA_TYPE_BINARY){
        if(IS_DEFINED(msg->msg_type,MSG_IPC_SHM)){
            printf("===============================================================\n");
            int i = 0;
            long size = msg->msg_data._long;
            unsigned char *pchar = (unsigned char *)msg->msg_shmdata;
            if(pchar == NULL) return;

            if(*(pchar +1) != 0){
                printf("Received a normal data : %s  size: %ld \n",msg->msg_shmid,msg->msg_data._long);
                usleep(10000);
                //return;
            }
            for(i = 0; i< size; i++){
                printf("%2X ",*(pchar + i));
                if(i % 20 == 0 && i != 0)
                    printf("\n");
            }
            //Release shared memory here ???
            //msg_shmrm(msg);
            printf("\nReceived a shm : %s  size: %ld \n",msg->msg_shmid,msg->msg_data._long);
            printf("===============================================================\n");
            if(*(pchar +1) == 0){
                exit(0);
            }
            return;
        }else{
             printf("Received a %s value: %s\n",msg->msg_name,msg->msg_data._buffer);
        }
    }else if(type == DATA_TYPE_ULONG){
        printf("Received a %s value: %lu\n",msg->msg_name,msg->msg_data._ulong);
    }else if(type == DATA_TYPE_ULLONG){
        printf("Received a %s value: %llu\n",msg->msg_name,msg->msg_data._ullong);
    }else if(type == DATA_TYPE_LLONG){
        printf("Received a %s value: %lld\n",msg->msg_name,msg->msg_data._llong);
    }else{
        printf("Received anther data type,it is undefined.\n");
    }
    
    snprintf(msg_buffer,sizeof(msg_buffer),"Thank you,I have received the messages.[%d]",count++);
    //Send welcome messages back to sender. 
    struct msg_st *srv_msg = msg_create_string("Welcome!",msg_buffer);
    //If you choose msg_send function to send message,you need to release memory by youself.
    //msg_send(msg->msg_module,&srv_msg,1); 
    msg_send_free(msg->msg_module,&srv_msg,10);

    return;
}
int main(int argc, char **argv)
{
    int ret = -1, loop = 1;

    ret = msg_init("radio_2",0,my_mq_recv);

    while(loop){
        sleep(1);
    }
    
    return ret;
}