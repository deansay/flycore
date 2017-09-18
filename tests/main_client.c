/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    main_client.c
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
    printf("msg->msg_type : %ld\n",msg->msg_type);
    printf("msg->msg_shmid : %s\n",msg->msg_shmid);
    printf("msg->msg_name : %s\n",msg->msg_name);
    printf("msg->msg_data : %s\n",msg->msg_data._buffer);
    printf("msg->msg_module : %s\n",msg->msg_module);
    printf("msg->msg_shmdata : %x\n",(unsigned int)(long)msg->msg_shmdata);
    return;
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
    int ret = -1;
    int count = 0;
    ret = msg_init("radio_3",0,my_mq_recv);
    
    //Data type transfer testing...
    char string[] = "Message data is binary.";
    msg_st *msg = msg_create_int("int",-1000);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_double("double",1.0/3.0);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_unsigned_int("unsigned int",0XFFFF0000);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_string("string","Message data is string.");
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_binary("binary",string,sizeof(string));
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_float("float",-1216334.1356565);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_short("short",-32768);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_long("long",-2147483648);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_char("char",'W');
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_unsigned_char("unsigned char",0XF0);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_unsigned_short("unsigned short",65535);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_unsigned_long("unsigned long",4294967295);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_unsigned_llong("unsigned long long",1844674407370955161u);
    msg_send_free("radio_2",&msg,10);
    msg = msg_create_llong("long long",-9223372036854775808LL);
    msg_send_free("radio_2",&msg,10);

    char *data = generate_huge_data(256);
    msg = msg_create_binary("binary",data,256);
    msg_send_free("radio_2",&msg,10);
    
    #if 1
    //Speed testing..
    while(1){
        count ++;
        char buff[128] = {};
        snprintf(buff,sizeof(buff),"Message count [%d].",count);
        msg = msg_create_string("message name you want.",buff);
        msg_send_free("radio_2",&msg,1);
        char *data = generate_huge_data(256);
        msg = msg_create_binary("binary",data,256);
        msg_send_free("radio_2",&msg,10);
        //usleep(20000);
        //sleep(1);
    }
    #endif
    return ret;
}