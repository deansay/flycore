/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    msgq.h
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-07-31 16:29:41
 * updated: 2017-07-31 16:29:41
 *****************************************************************************/
#ifndef MSGQ_H
#define MSGQ_H

#include <mqueue.h>
#include <pthread.h>

#define MAX_IPC_MESSAGES          5
#define MAX_IPC_MESSAGE_SIZE      1024
#define MQ_MSG_PRIO               10

#include "libipc.h"

#ifdef __cplusplus
extern "C" {
#endif
struct mq_ctx;

typedef enum ipc_role {
    IPC_RECEIVE = 0,
    IPC_SEND = 1,
}ipc_role;

typedef void (*mq_recv_cb) (struct msg_st *);
typedef void (*msg_recv_cb)(struct mq_ctx *mq_ctx,struct msg_st *msg);

typedef struct mq_ctx{
    mqd_t mqdes;
    long msgsize;
    long maxmsg;
    pthread_t thread_id;
    mq_recv_cb recv_cb;
    msg_recv_cb msg_cb;
    void *recv_buf;
}mq_ctx;



typedef void (mq_notify_cb)(union sigval sv);


mq_ctx *_mq_init(const char *name,mq_recv_cb recv_cb,msg_recv_cb msg_cb,ipc_role role);
void _mq_deinit(struct mq_ctx *mq_ctx,const char *name);
int _mq_send(struct mq_ctx *mq_ctx, const char *buf, long len,long msg_prio);
int _mq_recv(struct mq_ctx *mq_ctx, void *buf, long len);
int _mq_setattr(struct mq_ctx *mq_ctx);

#ifdef __cplusplus
}
#endif

#endif
