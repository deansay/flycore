/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    libipc.h
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-08-02 16:29:41
 * updated: 2017-08-02 16:29:41
 *****************************************************************************/
#ifndef LIBIPC_H
#define LIBIPC_H
#include <sys/time.h>
#include "shm.h"
#include "msgq.h"

#ifdef __cplusplus
extern "C" {
#endif

enum data_type{
    DATA_TYPE_INT    = 0,
    DATA_TYPE_DOUBLE = 1,
    DATA_TYPE_FLOAT  = 2,
    DATA_TYPE_SHORT  = 3,
    DATA_TYPE_LONG   = 4,
    DATA_TYPE_CHAR   = 5,
    DATA_TYPE_UINT   = 6,
    DATA_TYPE_USHORT = 7,
    DATA_TYPE_UCHAR  = 8,
    DATA_TYPE_STRING = 9,
    DATA_TYPE_JSON   = 10,
    DATA_TYPE_BINARY = 11,
    DATA_TYPE_ULONG  = 12,
    DATA_TYPE_ULLONG = 13,
    DATA_TYPE_LLONG  = 14,
    DATA_TYPE_MAX    = 15,
    DATA_TYPE_LIST   = 16,
};

#define MSG_IPC_SHM              (1<<9)
#define MSG_IPC_MQ               (1<<10)
#define MSG_IPC_SOCKET           (1<<11)
#define MSG_IPC_RESERVED         (1<<12)
#define MSG_IPC_SHM_RELEASE      (1<<13)
#define MSG_IPC_LIST_RELEASE     (1<<14)

#define MSG_IPC_CONNECT          (1<<16)
#define MSG_IPC_ACCEPT           (1<<17)


#define MAX_MSG_DATA_SIZE     128
#define MAX_MSG_NAME_SIZE     32
#define MAX_MODULE_NAME_SIZE  32
#define MAX_MQ_NAME_SIZE      (MAX_MODULE_NAME_SIZE +2)
#define MAX_MQ_NAME_SIZE      (MAX_MODULE_NAME_SIZE +2)
#define MAX_IPC_CLIENTS       256
#define MAX_SHM_NAME_SIZE     (MAX_MSG_NAME_SIZE + MAX_MODULE_NAME_SIZE + 3)


#define IS_DEFINED(val,def)  ((val&def) == def)
#define SET_DATA_TYPE(type,val)  (val |= (val&(~(val&0xFF)))| (type&0xFF))
#define GET_DATA_TYPE(type,val)  (type = val&0xFF)

typedef union _data{
    int     _int;
    double  _double;
    float   _float;
    short   _short;
    long    _long;
    char    _char;
    long long       _llong;
    unsigned int    _uint;
    unsigned short  _ushort;
    unsigned char   _uchar;
    unsigned long   _ulong;
    unsigned long long _ullong;
    char  _buffer[MAX_MSG_DATA_SIZE];
}_data;

typedef struct msg_st{
    long   msg_type;                         //0XFF << 8 is for ipc type,0XFF is for data type eg. double, int,other reserved.
    char   msg_shmid[MAX_SHM_NAME_SIZE];     //Huge data,using shared memory to transfer to other process, this is the shmid.
    char   msg_name[MAX_MSG_NAME_SIZE];      //A unique message name, it's will be set context attr at root in qmlengine.
    char   msg_module[MAX_MODULE_NAME_SIZE]; //Which module are messages from.
    _data  msg_data;                         //If msg_shmdata is available, msg_data is msg_shmdata size.
    long   msg_size;                         //If msg_shmdata is available, msg_size is msg_shmdata size.
    void   *msg_shmdata;
}msg_st;

typedef struct ipc_ctx{
    char   module[MAX_MODULE_NAME_SIZE];
    struct mq_ctx *mq_ctx;
    long   msg_prio;
}ipc_ctx;

/*
 * message queue IPC build flow:
 *
 *            
 * step.1 Init message queues at /dev/mqueue/module for module itself.
 *         create thread to wait message, if message arrived,then call back function.
 * step.2 Init message queues at /dev/mqueue/module_client for client.
 *        
 * step.3 Create message using msg_create_int for send a value of int data type.                          
 *        Maybe you can send all data using msg_create_binary. However you want.
 * step.4 send message ->msg_send or msg_send_free. msg_send_free will release the memories automatically.
 */
typedef void (*recv_cb) (struct msg_st *msg);

ipc_ctx *msg_init(const char* module,int cflag, recv_cb receive);
int msg_wait(struct ipc_ctx *ipc_ctx,const char* module,const struct timespec *abs_timeout); //If ads_timeout is null,that means no block.
int msg_send(struct ipc_ctx *ipc_ctx,const char* module,struct msg_st *msg);
int msg_send_free(struct ipc_ctx *ipc_ctx,const char *module,struct msg_st **msg); //After send message,it will free memorys automatically.
int msg_setattr(const char*module,long msgsize,long maxmsg);
void msg_deinit(const char* module);

//Message Help functions.
msg_st *msg_create_int(const char *msg_name,int       data);
msg_st *msg_create_double(const char *msg_name,double data);
msg_st *msg_create_short(const char *msg_name,short   data);
msg_st *msg_create_char(const char *msg_name,char      data);
msg_st *msg_create_long(const char *msg_name,long     data);
msg_st *msg_create_float(const char *msg_name,float   data);
msg_st *msg_create_llong(const char *msg_name,long long data);
msg_st *msg_create_string(const char *msg_name,const char *data);
msg_st *msg_create_unsigned_int(const char *msg_name,unsigned int      data);
msg_st *msg_create_unsigned_char(const char *msg_name,unsigned char    data);
msg_st *msg_create_unsigned_short(const char *msg_name,unsigned short data);
msg_st *msg_create_unsigned_long(const char *msg_name,unsigned long data);
msg_st *msg_create_unsigned_llong(const char *msg_name,unsigned long long data);
msg_st *msg_create_json(ipc_ctx *ipc_ctx,const char *msg_name,const char *data,size_t size);
msg_st *msg_create_binary(ipc_ctx *ipc_ctx,const char *msg_name,const void *data,size_t size);


#ifdef __cplusplus
}
#endif
#endif
