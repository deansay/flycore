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

void my_mq_recv(struct msg_st *msg)
{

    msg_st *_msg = msg_create_binary("binary",msg->msg_shmdata,msg->msg_size);
    msg_send_free(msg->msg_module,&_msg,10);
}
int main(int argc, char **argv)
{
    int ret = -1, loop = 1;

    ret = msg_init("T2",0,my_mq_recv);

    while(loop){
        sleep(1);
    }
    
    return ret;
}