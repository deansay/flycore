#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "log.h"

int db_error(int rc)
{
    switch(rc){
    case SXERR_MEM:         log_error("%s","Out of memory"); break;
    case SXERR_IO:          log_error("%s","IO error"); break;
    case SXERR_EMPTY:       log_error("%s","Empty field"); break;
    case SXERR_LOCKED:      log_error("%s","Locked operation"); break;
    case SXERR_ORANGE:      log_error("%s","Out of range value"); break;
    case SXERR_NOTFOUND:    log_error("%s","Item not found"); break;
    case SXERR_LIMIT:       log_error("%s","Limit reached"); break;
    case SXERR_MORE:        log_error("%s","Need more input"); break;
    case SXERR_INVALID:     log_error("%s","Invalid parameter"); break;
    case SXERR_ABORT:       log_error("%s","User callback request an operation abort"); break;
    case SXERR_EXISTS:      log_error("%s","Item exists"); break;
    case SXERR_SYNTAX:      log_error("%s","Syntax error"); break;
    case SXERR_UNKNOWN:     log_error("%s","Unknown error"); break;
    case SXERR_BUSY:        log_error("%s","Busy operation"); break;
    case SXERR_OVERFLOW:    log_error("%s","Stack or buffer overflow"); break;
    case SXERR_WILLBLOCK:   log_error("%s","Operation will block"); break;
    case SXERR_NOTIMPLEMENTED: log_error("%s","Operation not implemented"); break;
    case SXERR_EOF:         log_error("%s","End of input"); break;
    case SXERR_PERM:        log_error("%s","Permission error"); break;
    case SXERR_NOOP:        log_error("%s","No-op"); break;
    case SXERR_FORMAT:      log_error("%s","Invalid format"); break;
    case SXERR_NEXT:        log_error("%s","Not an error "); break;
    case SXERR_OS:          log_error("%s","System call return an error"); break;
    case SXERR_CORRUPT:     log_error("%s","Corrupted pointer"); break;
    case SXERR_CONTINUE:    log_error("%s","Not an error: Operation in progress"); break;
    case SXERR_NOMATCH:     log_error("%s","No match"); break;
    case SXERR_RESET:       log_error("%s","Operation reset"); break;
    case SXERR_DONE:        log_error("%s","Not an error"); break;
    case SXERR_SHORT:       log_error("%s","Buffer too short"); break;
    case SXERR_PATH:        log_error("%s","Path error "); break;
    case SXERR_TIMEOUT:     log_error("%s","Timeout"); break;
    case SXERR_BIG:         log_error("%s","Too big for processing "); break;
    case SXERR_RETRY:       log_error("%s","Retry your call "); break;
    case SXERR_IGNORE:      log_error("%s","Ignore"); break;
    default:break;
    }
    return rc;
}
/*
 * Extract the database error log and exit.
 */
void db_fatal(unqlite *db,const char *msg)
{
    if( db ){
        const char *error;
        int len = 0; /* Stupid cc warning */

        /* Extract the database error log */
        unqlite_config(db,UNQLITE_CONFIG_ERR_LOG,&error,&len);
        if( len > 0 ){
            /* Output the DB error log */
            log_error("%s",error); /* Always null termniated */
        }
    }else{
        if(msg){
            log_error("%s",msg);
        }
    }
}
/*Database init with memory and file type. */
db_ctx *db_init(const char *module, const char *db_path)
{
    db_ctx *ctx = (db_ctx *)malloc(sizeof(db_ctx));
    int rc;

    if(ctx == NULL){
        return NULL;
    }

    snprintf(ctx->db_module,sizeof(ctx->db_module),"%s",module);
    snprintf(ctx->db_store,sizeof(ctx->db_store),"%s/_%s.db",db_path,module);

    rc = unqlite_open(&ctx->db_file,ctx->db_store/* On-disk DB */,UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK ){
        db_error(rc);
        free(ctx);
        return NULL;
    }

    rc = unqlite_open(&ctx->db_mem,":mem:",UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK ){
        db_error(rc);
        free(ctx);
        return NULL;
    }

    return ctx;
}

int db_close(db_ctx *db)
{
    int rc = unqlite_close(db->db_file);
    if( rc != UNQLITE_OK ){
        db_error(rc);
    }
    db->db_file = NULL;
    return rc;
}

int db_open(db_ctx *ctx)
{
    int rc;

    if(ctx->db_file != NULL)
         return -1;

    rc = unqlite_open(&ctx->db_file,ctx->db_store/* On-disk DB */,UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK ){
        db_error(rc);
    }

    return rc;
}
int mem_open(db_ctx *ctx)
{
    int rc;

    if(ctx->db_mem != NULL)
         return -1;

    rc = unqlite_open(&ctx->db_mem,":mem:",UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK ){
        db_error(rc);
    }
    return rc;
}

int mem_close(db_ctx *db)
{
    int rc = unqlite_close(db->db_file);
    if( rc != UNQLITE_OK ){
        db_error(rc);
    }
    db->db_mem = NULL;
    return rc;
}
 /* Auto-commit the transaction and close our database */
int db_deinit(db_ctx *db)
{
    unqlite_close(db->db_file);
    db->db_file = NULL;
    unqlite_close(db->db_mem);
    db->db_mem = NULL;
    free(db);

    return 0;
}
