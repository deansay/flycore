/******************************************************************************
 * Copyright (C) Dean Zou 2016-2017
 * file:    shm.h
 * author:  Dean Zou <deboot@msn.cn>
 * created: 2017-07-31 16:29:41
 * updated: 2017-07-31 16:29:41
 *****************************************************************************/
#ifndef SHM_H
#define SHM_H

#ifdef __cplusplus
extern "C" {
#endif

#define SHM_MAX_CLINETS  128
#define SHM_ID_UNIQUE    1
#define SHM_ID_READONLY  0

struct shm_ctx {
    void *addr;
    char name[256];
    long size;
    int  fd;
};

struct shm_ctx *shm_init(const char *name, long size,int unique);
int shm_write(struct shm_ctx *shm_ctx, const void *buf, long len,long pos);
int shm_read(struct shm_ctx *shm_ctx, void *buf, long len,long pos);
int shm_close(struct shm_ctx *shm_ctx);
int shm_deinit(struct shm_ctx *shm_ctx);

#ifdef __cplusplus
}
#endif
#endif
