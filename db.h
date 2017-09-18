#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <stddef.h>
#include <limits.h>

#include "unqlite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct db_ctx{
    unqlite *db_mem;
    unqlite *db_file;
    char    db_module[PATH_MAX];
    char    db_store[PATH_MAX];
}db_ctx;

#define MAX_DB_STORE_SIZE  1024

typedef int (*xconsumer)(const void *,unsigned int,void *);

db_ctx *db_init(const char *module,const char *root_path);
int db_close(db_ctx *db);
int db_open(db_ctx *ctx);
int mem_close(db_ctx *db);
int mem_open(db_ctx *ctx);
int db_deinit(db_ctx *db);
int db_error(int rc);

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
# if defined __GNUC__ && __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#define db_store(db, key,data, len) \
    db_error(unqlite_kv_store(db->db_file, key,-1,data,len));

#define db_fetch(db, key,data, len) \
    db_error(unqlite_kv_fetch(db->db_file, key,-1,data,len));

#define db_store_fmt(db, key,format, ...) \
    db_error(unqlite_kv_store_fmt(db->db_file, key,-1, format, __VA_ARGS__));

#define db_fetch_cb(db, key,consumer, userdata) \
    db_error(unqlite_kv_fetch_callback(db->db_file, key,-1, consumer, userdata));

#define db_append(db, key,data, len) \
    db_error(unqlite_kv_append(db->db_file, key,-1,data,len));

#define db_append_fmt(db, key,format, ...) \
    db_error(unqlite_kv_append_fmt(db->db_file, key,-1, format, __VA_ARGS__));
#define db_random_key(db,key,size)\
    db_error(unqlite_util_random_string(db->db_file,key,size));

#define db_rollback(db,key)\
    db_error(unqlite_rollback(db->db_file));

#define mem_store(db, key,data, len) \
    db_error(unqlite_kv_store(db->db_mem, key,-1,data,len));

#define mem_fetch(db, key,data, len) \
    db_error(unqlite_kv_fetch(db->db_mem, key,-1,data,len));

#define mem_store_fmt(db, key,format, ...) \
    db_error(unqlite_kv_store_fmt(db->db_mem, key,-1, format, __VA_ARGS__));

#define mem_fetch_cb(db, key,consumer, userdata) \
    db_error(unqlite_kv_fetch_callback(db->db_mem, key,-1, consumer, userdata));

#define mem_append(db, key,data, len) \
    db_error(unqlite_kv_append(db->db_mem, key,-1,data,len));

#define mem_append_fmt(db, key,format, ...) \
    db_error(unqlite_kv_append_fmt(db->db_mem, key,-1, format, __VA_ARGS__));
#define mem_random_key(db,key,size)\
    db_error(unqlite_util_random_string(db->db_mem,key,size));

#define mem_rollback(db,key)\
    db_error(unqlite_rollback(db->db_mem));
#elif defined __GNUC__
#define db_store(db, key,data, len) \
    db_error(unqlite_kv_store(db->db_file, key,-1,data,len));

#define db_fetch(db, key,data, len) \
    db_error(unqlite_kv_fetch(db->db_file, key,-1,data,len));

#define db_store_fmt(db, key,format, args...) \
    db_error(unqlite_kv_store_fmt(db->db_file, key,-1, format, ##args));

#define db_fetch_cb(db, key,consumer, userdata) \
    db_error(unqlite_kv_fetch_callback(db->db_file, key,-1, consumer, userdata));

#define db_append(db, key,data, len) \
    db_error(unqlite_kv_append(db->db_file, key,-1,data,len));

#define db_append_fmt(db, key,format, args...) \
    db_error(unqlite_kv_append_fmt(db->db_file, key,-1, format, ##args));
#define db_random_key(db,key,size)\
    db_error(unqlite_util_random_string(db->db_file,key,size));

#define db_rollback(db,key)\
    db_error(unqlite_rollback(db->db_file));

#define mem_store(db, key,data, len) \
    db_error(unqlite_kv_store(db->db_mem, key,-1,data,len));

#define mem_fetch(db, key,data, len) \
    db_error(unqlite_kv_fetch(db->db_mem, key,-1,data,len));

#define mem_store_fmt(db, key,format, args...) \
    db_error(unqlite_kv_store_fmt(db->db_mem, key,-1, format, ##args));

#define mem_fetch_cb(db, key,consumer, userdata) \
    db_error(unqlite_kv_fetch_callback(db->db_mem, key,-1, consumer, userdata));

#define mem_append(db, key,data, len) \
    db_error(unqlite_kv_append(db->db_mem, key,-1,data,len));

#define mem_append_fmt(db, key,format, args...) \
    db_error(unqlite_kv_append_fmt(db->db_mem, key,-1, format, ##args));
#define mem_random_key(db,key,size)\
    db_error(unqlite_util_random_string(db->db_mem,key,size));

#define mem_rollback(db,key)\
    db_error(unqlite_rollback(db->db_mem));

#endif
#ifdef __cplusplus
}
#endif
#endif // DB_H
