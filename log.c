#include "log.h"

zlog_category_t *_category = NULL;
zlog_category_t *_hex_category = NULL;

#define LOG_CATEGORY_HEX_NAME "firefly_hex"
#define LOG_CATEGORY_LOG_NAME "firefly_log"

#define LOG_CONFIG_DEFUALT_CONFG      "/tmp/firefly_log.conf"

int log_create_default(const char*file)
{
    FILE *fp=fopen(file,"w");

    if(fp == NULL)
        return -1;

    fputs("[global]\n",fp);
    fputs("strict init = true\n",fp);
    fputs("buffer max  = 2MB\n",fp);
    fputs("rotate lock file	=/tmp/zlog.lock\n",fp);
    fputs("default format=		\"Firefly - %d(%F %X.%ms) %-6V (%c:%F:%U:%L) - %m%n\"\n",fp);
    fputs("\n",fp);
    fputs("[formats]\n",fp);
    fputs("normal =	\"Firefly - %d(%F %X.%ms) %-6V (%F:%U:%L) - %m%n\"\n",fp);
    fputs("[rules]\n",fp);
    fputs("firefly_log.DEBUG	\"/tmp/Firefly-debug.log\",10MB; normal\n",fp);
    fputs("firefly_log.INFO	\"/tmp/Firely-info.log\",10MB; normal\n",fp);
    fputs("firefly_log.NOTICE	\"/tmp/Firefly-notice.log\",10MB; normal\n",fp);
    fputs("firefly_log.WARN	\"/tmp/Firefly-warn.log\",10MB; normal\n",fp);
    fputs("firefly_log.ERROR	\"/tmp/Firefly-error.log\",10MB; normal\n",fp);
    fputs("firefly_log.FATAL	\"/tmp/Firefly-fatal.log\",10MB; normal\n",fp);
    fputs("firefly_hex.*	\"/tmp/Firefly-hex.log\",10MB;\n",fp);

    fclose(fp);
    return 0;
}

int log_init(const char *confpath)
{
    int ret = -1;

    if(confpath == NULL){
        log_create_default(LOG_CONFIG_DEFUALT_CONFG);
        ret = zlog_init(LOG_CONFIG_DEFUALT_CONFG);
    }else{
        ret = zlog_init(confpath);
    }

    if (ret) {
        printf("Firefly log init failed.\n");
        return -1;
    }

    _category = zlog_get_category(LOG_CATEGORY_LOG_NAME);
    if (!_category) {
        printf("Firefly log get %s category failed.\n",LOG_CATEGORY_LOG_NAME);
        zlog_fini();
        return -2;
    }

    _hex_category = zlog_get_category(LOG_CATEGORY_HEX_NAME);
    if (!_category) {
        printf("Firefly log get %s category failed.\n",LOG_CATEGORY_HEX_NAME );
        zlog_fini();
        return -2;
    }

   ret = dzlog_init(confpath, LOG_CATEGORY_LOG_NAME);

    return ret;
}

int log_reload(const char *confpath)
{
    int ret = -1;
    ret = zlog_reload(confpath);
    if (ret) {
        printf("Firefly log update fail. \n");
    }
    zlog_info(_category, "Firefly log after update. ");
    zlog_profile();
    zlog_fini();
    log_init(confpath);

    return ret;
}

void log_deinit()
{
    zlog_fini();
}
