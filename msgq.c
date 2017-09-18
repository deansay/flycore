/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    msgq.h
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-07-31 16:29:41
 * updated: 2017-07-31 16:29:41
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include "log.h"
#include "msgq.h"
#include "libipc.h"

#define FILE_MODE (S_IRWXU | S_IRWXG | S_IRWXO)
static mqd_t _mq_open(const char *name, int flag);
static void *_mq_on_recv_routine(void *ctx);
static int _mq_shm_read(struct msg_st *msg);

static mqd_t _mq_open(const char *name, int flag)
{
    mqd_t msgq_id;
    struct mq_attr attr;
    //int rflag = O_RDONLY | O_CREAT | O_NONBLOCK;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_IPC_MESSAGES;
    attr.mq_msgsize = MAX_IPC_MESSAGE_SIZE;
    attr.mq_curmsgs = 0;

    /*
    //Please consider this user case,if receiver start after sender, it will not receive a message from sender ever.
    //If undo mq_unlink, the sender will wait for the receiver is ready.
    if(IS_DEFINED(flag, O_CREAT)){
        printf("%s message queue is a receive queue,so unlink previous queue before open.\n",name);
        mq_unlink(name);
    }
    */
    msgq_id = mq_open(name, flag, FILE_MODE, &attr);
    
    if(msgq_id == (mqd_t)-1){
        log_error("%s message queue create failed. %s",name,strerror(errno));
        return msgq_id;
    }

    log_info("Queue \"%s\":\n\t- stores at most %ld messages\n\t- \
    large at most %ld bytes each\n\t- currently holds %ld messages\n", 
    name, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    return msgq_id;
}

mq_ctx* _mq_init(const char *name,mq_recv_cb recv_cb,msg_recv_cb msg_cb,enum ipc_role role)
{
    struct mq_ctx *mq_ctx = NULL;
    mq_ctx = (struct mq_ctx *)calloc(1,sizeof(struct mq_ctx));

    if(mq_ctx == NULL){
        log_error("%s",strerror(errno));
        return NULL;
    }

    if(role == IPC_RECEIVE){
        mq_ctx->mqdes = _mq_open(name,O_CREAT|O_RDWR);
    }else if(role == IPC_SEND){
        mq_ctx->mqdes = _mq_open(name,O_CREAT|O_WRONLY);
    }else{
         log_error("undefined IPC role");
    }

    mq_ctx->msgsize = MAX_IPC_MESSAGE_SIZE;
    mq_ctx->maxmsg = MAX_IPC_MESSAGES;
    mq_ctx->recv_cb = recv_cb;
    mq_ctx->msg_cb = msg_cb;

    if(mq_ctx->mqdes == -1){
        free(mq_ctx);
        return NULL;
    }

    if(role == IPC_RECEIVE){
        if(pthread_create(&mq_ctx->thread_id,NULL,_mq_on_recv_routine,mq_ctx)!=0){
            log_error("%s",strerror(errno));
        }
    }
    return mq_ctx;
}

void _mq_deinit(struct mq_ctx *mq_ctx,const char *name)
{
    mq_close(mq_ctx->mqdes);
    mq_unlink(name);
    free(mq_ctx);
}
int _mq_send(struct mq_ctx *mq_ctx, const char *buf, long len,long msg_prio)
{
    if(0 != mq_send(mq_ctx->mqdes, buf, len, msg_prio)){
        log_error("mq_send failed %d: %s\n", errno, strerror(errno));
        return -1;
    }

    return len;
}

static int _mq_shm_read(struct msg_st *msg)
{
    int ret = -1;
    struct shm_ctx *shm_ctx = NULL;
    long size = 0;
    char *data = NULL;
    //If data is from shared memory, msg_data is size of this memroy.
    size = msg->msg_data._long;
    data = (char *)malloc(size); 
    msg->msg_shmdata = NULL;
    
    if(data == NULL) {
        log_error("%s",strerror(errno));
        goto error;
    }
    
    shm_ctx = shm_init(msg->msg_shmid,size,SHM_ID_READONLY);

    if(shm_ctx == NULL){
        log_error("%s",strerror(errno));
        free(data);
        goto error;
    }

    shm_read(shm_ctx,data,size,0);
    msg->msg_shmdata = data;
    shm_deinit(shm_ctx);
    ret = 0;
error:
    return ret;
}

void *_mq_on_recv_routine(void *ctx)
{
    int len;
    int loop = 1;
    void *recv_buf = NULL;
    struct mq_ctx *mq_ctx = (struct mq_ctx *)ctx;

    log_info("Message queue notify thread is started\n");
    recv_buf = calloc(1,mq_ctx->msgsize);

    if(recv_buf == NULL){
        log_error("%s",strerror(errno));
        return NULL;
    }

    while(loop){
        memset(recv_buf,0,mq_ctx->msgsize);
        //recv len must greater than mq_msgsize;
        len = _mq_recv(mq_ctx, recv_buf, mq_ctx->msgsize);
        int msg_len = sizeof(msg_st); //disable warnning..
        if (len < msg_len) {
            log_error("_mq_recv failed! len = %d\n", len);
            goto error;
        }else{
            //printf("Message queue receive data [%d] bytes\n", len);
        }

        struct msg_st *recv_msg = (struct msg_st *)recv_buf;

        if(IS_DEFINED(recv_msg->msg_type,MSG_IPC_SHM_RELEASE))
        {
            char shm_path[64] ={};
            snprintf(shm_path,sizeof(shm_path),"/dev/shm/%s",recv_msg->msg_shmid);
            shm_unlink(recv_msg->msg_shmid);
            remove(shm_path);
            log_info("Removing shared memory with shmid %s\n",recv_msg->msg_shmid );
            continue;
        }

        if (mq_ctx->msg_cb) {
            
            if(IS_DEFINED(recv_msg->msg_type,MSG_IPC_SHM)){
                _mq_shm_read(recv_msg);
            }else{
                recv_msg->msg_shmdata = &recv_msg->msg_data;
            }
            
            mq_ctx->msg_cb(mq_ctx,recv_msg);

            if(IS_DEFINED(recv_msg->msg_type,MSG_IPC_SHM) && recv_msg->msg_shmdata != NULL)
                free(recv_msg->msg_shmdata);
        }
    }
error:
    free(recv_buf);
    return NULL;
}
int _mq_recv(struct mq_ctx *mq_ctx, void *buf, long len)
{
    ssize_t ret = mq_receive(mq_ctx->mqdes, (char *)buf, len, NULL);
    if (-1 == ret) {
        log_error("mq_receive failed %d: %s\n", errno, strerror(errno));
        return -1;
    }
    return ret;
}
int _mq_setattr(struct mq_ctx *mq_ctx)
{
    struct mq_attr msgq_attr;
    if(mq_getattr(mq_ctx->mqdes, &msgq_attr) == -1){
        log_error("%s",strerror(errno));
        return -1;
    }
    log_info("Queue \"%s\":\n\t- stores at most %ld messages\n\t- \
    large at most %ld bytes each\n\t- currently holds %ld messages\n", 
    "name", msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

    msgq_attr.mq_maxmsg = mq_ctx->maxmsg;
    msgq_attr.mq_msgsize = mq_ctx->msgsize;

    if(mq_setattr(mq_ctx->mqdes, &msgq_attr, NULL) == -1)
        return -1;

    return 0;
}
