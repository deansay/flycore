/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    ipc_private.h
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-08-02 16:29:41
 * updated: 2017-08-02 16:29:41
 *****************************************************************************/
#ifndef IPC_PRIVATE_H
#define IPC_PRIVATE_H

#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>

#include "libipc.h"
#include "msgq.h"
#include "list.h"

int list_node_compare(list_node*list,const void*data);
void msg_shmrm(msg_st *received_msg);
void msg_recv_callback(mq_ctx *mq_ctx,struct msg_st *msg);

#endif
