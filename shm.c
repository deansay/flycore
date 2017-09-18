/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    shm.c
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-07-31 16:29:41
 * updated: 2017-07-31 16:29:41
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "shm.h"
#include "log.h"

struct shm_ctx *shm_init(const char *name, long size,int unique)
{
    int fd;
    int ret = -1;
    struct shm_ctx *shm_ctx = NULL;
    void *addr = NULL;
    if(unique == SHM_ID_UNIQUE){
        fd = shm_open(name, O_CREAT|O_EXCL|O_RDWR, 00777);
        if(fd == -1){
            log_error("shm_init->shm_open %s failed. %s unique = %d\n",name,strerror(errno),unique);
            goto error2;
        }
    }else{
        fd = shm_open(name, O_CREAT|O_RDWR, 00777);
        if(fd == -1){
            log_error("shm_init->shm_open %s failed. %s unique = %d\n",name,strerror(errno),unique);
            goto error2;
        }
    }

    ret = ftruncate(fd, size);

    if(ret  == -1){
        log_error("shm_init->ftruncate failed. %s\n",strerror(errno));
        goto error1;
    }
    
    addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, SEEK_SET);
    if(addr == NULL){
         log_error("shm_init->mmap failed. %s\n",strerror(errno));
         goto error1;
    }

    shm_ctx = (struct shm_ctx *)calloc(1,sizeof(struct shm_ctx));

    if(shm_ctx == NULL)
        goto error0;

    shm_ctx->addr = addr;
    shm_ctx->fd = fd;
    shm_ctx->size = size;
    strcpy(shm_ctx->name,name);

    return shm_ctx;

error0:
    munmap(shm_ctx->addr, shm_ctx->size);
error1:
    close(fd);
    shm_unlink(name);
error2:
    return shm_ctx;
}

int shm_close(struct shm_ctx *shm_ctx)
{
    int ret = -1;
    ret = munmap(shm_ctx->addr, shm_ctx->size);
    close(shm_ctx->fd);
    free(shm_ctx);

    return ret;
}
int shm_deinit(struct shm_ctx *shm_ctx)
{
    int ret = -1;

    ret = munmap(shm_ctx->addr, shm_ctx->size);
    close(shm_ctx->fd);
    ret = shm_unlink(shm_ctx->name);
    
    free(shm_ctx);

    return ret;
}


int shm_write(struct shm_ctx *shm_ctx, const void *buf, long len,long pos)
{
    memcpy(shm_ctx->addr + pos,buf, len);

    return len;
}

int shm_read(struct shm_ctx *shm_ctx, void *buf, long len,long pos)
{
    memcpy(buf,shm_ctx->addr + pos, len);

    return len;
}
