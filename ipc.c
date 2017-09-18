/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    libipc.c
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-08-02 16:29:41
 * updated: 2017-08-02 16:29:41
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "libipc.h"
#include "ipc_private.h"
#include "msgq.h"

list_node *ipc_ctx_list = NULL; //Global value for IPC list.

/**
 * msg_init() - init
 * @module:              
 * @cflag:              
 * @receive:            
 * Return: 0 on success, a negative error code on failure.
 */
ipc_ctx *msg_init(const char *module,int cflag,recv_cb receive)
{
    ipc_ctx *ipc_ctx = NULL;
    char shm_rm_cmd[128] ={};
    if(strlen(module) < 1){
        log_error("Module name must be greater then one character.\n");
        return NULL;
    }

    log_info("Module: %s %d initializing...\n",module,cflag);

    if(ipc_ctx_list != NULL){
        if(list_find(ipc_ctx_list,list_node_compare,module) == NULL){
            log_error("%s has already initialized.\n",module);
            return NULL;
        }
    }
    else{
        ipc_ctx = (struct ipc_ctx*)calloc(1,sizeof(struct ipc_ctx));
        if(ipc_ctx == NULL){
            log_error("Module: %s init failed, memory is not enough.\n",module);
            return NULL;
        }
    }

    snprintf(ipc_ctx->module, sizeof(ipc_ctx->module), "/%s", module);

    //Remove shared memory from system before starting message queues.
    snprintf(shm_rm_cmd,sizeof(shm_rm_cmd),"rm -rf /dev/shm/%s.*",module);
    system(shm_rm_cmd);

    ipc_ctx->mq_ctx   = _mq_init(ipc_ctx->module,(mq_recv_cb)receive,msg_recv_callback,IPC_RECEIVE);

    if(ipc_ctx->mq_ctx == NULL){
        free(ipc_ctx);
        log_error("Module: %s init failed, message queues init failed.\n",module);
        return NULL;
    }
    if(ipc_ctx->mq_ctx->mqdes == -1){
        free(ipc_ctx);
        log_error("Module: %s init failed, message queues init failed.\n",module);
    }

    ipc_ctx->msg_prio = 10;

    ipc_ctx_list = list_create(ipc_ctx);

    log_info("Module: %s initializing successfully...\n",module);

    return ipc_ctx;
}

/**
 * msg_wait() - init
 * @module:              
 * @timespec:                      
 *
 * 
 *
 * Return: 0 on success, a negative error code on failure.
 */
int msg_wait(ipc_ctx *ipc_ctx,const char *module,const struct timespec *abs_timeout)
{
    return 0;
}
/**
 * msg_send() - init
 * @module:              
 * @timespec:                      
 *
 *
 *
 * Return: 0 on success, a negative error code on failure.
 */
int msg_send_free(ipc_ctx *ipc_ctx,const char *module,struct msg_st **msg)
{
    int ret = -1;
    if(msg == NULL && ipc_ctx == NULL)
        return ret;

    if(*msg != NULL){
        ret = msg_send(ipc_ctx,module,*msg);

        if((*msg)->msg_shmdata != NULL)
            free((*msg)->msg_shmdata);
        free(*msg);
        *msg = NULL;
    }

    return ret;
}

int msg_send(ipc_ctx *ipc_ctx,const char *module,struct msg_st *msg)
{
    int ret = -1;
    int max_len = strlen(module);
    struct ipc_ctx *dest_ipc_ctx = NULL;
    char real_module_name[MAX_MODULE_NAME_SIZE]= {};

    if(max_len < 1 || max_len > MAX_MODULE_NAME_SIZE){
        log_error("Module name must be greater then one character.\n");
        return -1;
    }

    if(msg == NULL)
        return -1;

    snprintf(real_module_name,sizeof(real_module_name),"/%s",module);
    list_node *node = list_find(ipc_ctx_list,list_node_compare,real_module_name);

    if(node == NULL){
        dest_ipc_ctx = (struct ipc_ctx* )malloc(sizeof(struct ipc_ctx));
        if(dest_ipc_ctx == NULL)
            return -1;
        snprintf(dest_ipc_ctx->module, sizeof(dest_ipc_ctx->module), "/%s", module);
        //Saving this handle for sending messages,so no need receive callback.
        dest_ipc_ctx->mq_ctx = _mq_init(ipc_ctx->module,NULL,NULL,IPC_SEND);
        if(dest_ipc_ctx->mq_ctx == NULL){
            free(dest_ipc_ctx);
            return -1;
        }
        list_insert_after(ipc_ctx_list,dest_ipc_ctx);
    }else{
        dest_ipc_ctx= (struct ipc_ctx *)node->data;
    }
    if(dest_ipc_ctx != NULL){
        if(ipc_ctx != NULL)
            snprintf(msg->msg_module,sizeof(ipc_ctx->module),"%s",ipc_ctx->module);
        ret = _mq_send(dest_ipc_ctx->mq_ctx,(const char *)msg,sizeof(struct msg_st),ipc_ctx->msg_prio);
        return ret;
    }

    return 0;
}
/**
 * msg_setattr() - init
 * @module:              
 * @timespec:                      
 *
 * 
 *
 * Return: 0 on success, a negative error code on failure.
 */
int msg_setattr(const char*module,long msgsize,long maxmsg)
{
    char real_module_name[MAX_MODULE_NAME_SIZE]= {};
    snprintf(real_module_name,sizeof(real_module_name),"/%s",module);
    list_node *node = list_find(ipc_ctx_list,list_node_compare,real_module_name);
    ipc_ctx *ipc_ctx= (struct ipc_ctx *)node->data;
    if(ipc_ctx != NULL){
        ipc_ctx->mq_ctx->msgsize = msgsize;
        ipc_ctx->mq_ctx->maxmsg = maxmsg;

    }
    
    return _mq_setattr(ipc_ctx->mq_ctx);
}

int msg_ctx_free(list_node *list,void *data)
{
    ipc_ctx *ipc_ctx= (struct ipc_ctx *)list->data;
    _mq_deinit(ipc_ctx->mq_ctx,ipc_ctx->module);
    free(ipc_ctx);
    return 0;
}
void msg_deinit(const char *module)
{
    list_foreach(ipc_ctx_list,msg_ctx_free,NULL);
    list_destroy(&ipc_ctx_list);
}
void msg_shmrm(msg_st *received_msg)
{
    //Clear MSG_IPC_SHM flag then send back.
    received_msg->msg_type &= ~MSG_IPC_SHM;
    received_msg->msg_type |= MSG_IPC_SHM_RELEASE;
    msg_send(NULL,received_msg->msg_module,received_msg);
}
#define msg_create_private(name, data,type)                                 \
    struct msg_st *msg = (struct msg_st *)calloc(1,sizeof(struct msg_st));  \
    if(msg == NULL) return NULL;                                            \
    SET_DATA_TYPE(type,msg->msg_type);                                      \
    snprintf(msg->msg_name, sizeof(msg->msg_name), "%s", msg_name);         \
    memcpy(&msg->msg_data,&data,sizeof(data));                              \
    msg->msg_size = sizeof(data)                                            \

/**
 * msg_create_int() - init
 * @module:              
 * @timespec:                      
 *
 * 
 *
 * Return: 0 on success, a negative error code on failure.
 */
msg_st *msg_create_int(const char *msg_name,int data)
{
    msg_create_private(msg_name,data,DATA_TYPE_INT);
    return msg;
}
msg_st *msg_create_double(const char *msg_name,double data)
{
    msg_create_private(msg_name,data,DATA_TYPE_DOUBLE);
    return msg;
}

msg_st *msg_create_short(const char *msg_name,short data)
{
    msg_create_private(msg_name,data,DATA_TYPE_SHORT);
    return msg;
}

msg_st *msg_create_char(const char *msg_name,char data)
{
    msg_create_private(msg_name,data,DATA_TYPE_CHAR);
    return msg;
}
msg_st *msg_create_long(const char *msg_name,long data)
{
    msg_create_private(msg_name,data,DATA_TYPE_LONG);
    return msg;
}

msg_st *msg_create_float(const char *msg_name,float data)
{
    msg_create_private(msg_name,data,DATA_TYPE_FLOAT);
    return msg;
}

msg_st *msg_create_string(const char *msg_name,const char *data)
{
    long size = 0;
    if(data == NULL) return NULL;

    size = strlen(data)+1;

    struct msg_st *msg = (struct msg_st *)calloc(1,sizeof(struct msg_st));

    if(msg == NULL) return NULL;
    
    SET_DATA_TYPE(DATA_TYPE_STRING,msg->msg_type);
    snprintf(msg->msg_name, sizeof(msg->msg_name), "%s", msg_name);

    memcpy(msg->msg_data._buffer,data,size);
    msg->msg_size = size;

    return msg;
}
msg_st *msg_create_unsigned_int(const char *msg_name,unsigned int data)
{
    msg_create_private(msg_name,data,DATA_TYPE_UINT);
    return msg;
}
msg_st *msg_create_unsigned_char(const char *msg_name,unsigned char data)
{
    msg_create_private(msg_name,data,DATA_TYPE_UCHAR);
    return msg;
}
msg_st *msg_create_unsigned_short(const char *msg_name,unsigned short data)
{
    msg_create_private(msg_name,data,DATA_TYPE_USHORT);
    return msg;
}
msg_st *msg_create_llong(const char *msg_name,long long data)
{
    msg_create_private(msg_name,data,DATA_TYPE_LLONG);
    return msg;
}
msg_st *msg_create_unsigned_long(const char *msg_name,unsigned long data)
{
    msg_create_private(msg_name,data,DATA_TYPE_ULONG);
    return msg;
}
msg_st *msg_create_unsigned_llong(const char *msg_name,unsigned long long data)
{
    msg_create_private(msg_name,data,DATA_TYPE_ULLONG);
    return msg;
}
/**
 * msg_create_json() - init
 * @module:              
 * @timespec:                      
 *
 * 
 *
 * Return: 0 on success, a negative error code on failure.
 */
struct msg_st *msg_create_json(struct ipc_ctx *ipc_ctx,const char *msg_name,const char *data,size_t size)
{
    struct msg_st *msg = msg_create_binary(ipc_ctx,msg_name,data,size);
    if(msg != NULL)
        SET_DATA_TYPE(DATA_TYPE_JSON,msg->msg_type);

    return msg;
}
struct msg_st *msg_create_binary(ipc_ctx *ipc_ctx,const char *msg_name,const void *data,size_t size)
{
    struct msg_st *msg = (struct msg_st *)calloc(1,sizeof(struct msg_st));
 
    SET_DATA_TYPE(DATA_TYPE_BINARY,msg->msg_type);
    snprintf(msg->msg_name, sizeof(msg->msg_name), "%s", msg_name);
    msg->msg_size = size;
    if(size > sizeof(msg->msg_data)){
        char shm_name[MAX_SHM_NAME_SIZE] ={};
        struct shm_ctx *shm_ctx = NULL;
        static int  sed = 0;
        sed = sed > 10000 ? 0: sed;
        
        snprintf(shm_name,sizeof(shm_name),"%s.%s_%d",ipc_ctx->module,msg_name,sed++);
        shm_ctx = shm_init(shm_name,size,SHM_ID_UNIQUE);

        if(shm_ctx != NULL){
            snprintf(msg->msg_shmid,sizeof(shm_name),"%s",shm_name);
            shm_write(shm_ctx,data,size,0);
            msg->msg_type |= MSG_IPC_SHM;
            //shm_close does not remove the shared memory from system.
            //So it must be closed after sending remove message back from receiver.
            //This case is very critical.
            shm_close(shm_ctx);
            msg->msg_shmdata = (void *)data;
            msg->msg_data._long = size;
        }else{
            free(msg);
            return NULL;
        }
        
    }else{
        memcpy(msg->msg_data._buffer,data,size);
    }
    return msg;
}

int list_node_compare(list_node *list,const void *data)
{
    struct ipc_ctx *ipc_ctx = (struct ipc_ctx*)list->data;
    return strcmp(ipc_ctx->module,(char *)data) ? 0:1;
}

void msg_recv_callback(mq_ctx *mq_ctx,msg_st *msg)
{
    if(mq_ctx->recv_cb)
        mq_ctx->recv_cb(msg);
    if(IS_DEFINED(msg->msg_type,MSG_IPC_SHM))
        msg_shmrm(msg);
}
