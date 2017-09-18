#ifndef LOG_H
#define LOG_H

#include "zlog/zlog.h"

#ifdef __cplusplus
extern "C" {
#endif

extern zlog_category_t *_category;
extern zlog_category_t *_hex_category;

int log_init(const char *confpath);
int log_reload(const char *confpath);
void log_deinit(void);

# if defined __GNUC__
#   define ZLOG_CHECK_PRINTF(m,n) __attribute__((format(printf,m,n)))
# else
#   define ZLOG_CHECK_PRINTF(m,n)
# endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
# if defined __GNUC__ && __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
/* zlog macros */
#define log_fatal(cat, ...) \
    zlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, __VA_ARGS__)
#define log_error(cat, ...) \
    zlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, __VA_ARGS__)
#define log_warn(cat, ...) \
    zlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, __VA_ARGS__)
#define log_notice(cat, ...) \
    zlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, __VA_ARGS__)
#define log_info(cat, ...) \
    zlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, __VA_ARGS__)
#define log_debug(cat, ...) \
    zlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, __VA_ARGS__)
/* dzlog macros */
#define dlog_fatal(...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, __VA_ARGS__)
#define dlog_error(...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, __VA_ARGS__)
#define dlog_warn(...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, __VA_ARGS__)
#define dlog_notice(...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, __VA_ARGS__)
#define dlog_info(...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, __VA_ARGS__)
#define dlog_debug(...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, __VA_ARGS__)
#elif defined __GNUC__
/* zlog macros */
#define log_fatal(format, args...) \
    zlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, format, ##args)
#define log_error(format, args...) \
    zlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, format, ##args)
#define log_warn(format, args...) \
    zlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, format, ##args)
#define log_notice(format, args...) \
    zlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, format, ##args)
#define log_info(format, args...) \
    zlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, format, ##args)
#define log_debug(format, args...) \
    zlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, format, ##args)
/* dzlog macros */
#define dlog_fatal(format, args...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, format, ##args)
#define dlog_error(format, args...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, format, ##args)
#define dlog_warn(format, args...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, format, ##args)
#define dlog_notice(format, args...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, format, ##args)
#define dlog_info(format, args...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, format, ##args)
#define dlog_debug(format, args...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, format, ##args)
#endif

/* vzlog macros */
#define vlog_fatal(format, args) \
    vzlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, format, args)
#define vlog_error(format, args) \
    vzlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, format, args)
#define vlog_warn(format, args) \
    vzlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, format, args)
#define vlog_notice(format, args) \
    vzlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, format, args)
#define vlog_info(format, args) \
    vzlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, format, args)
#define vlog_debug(format, args) \
    vzlog(_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, format, args)

/* hzlog macros */
#define hlog_fatal(buf, buf_len) \
    hzlog(_hex_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, buf, buf_len)
#define hlog_error(buf, buf_len) \
    hzlog(_hex_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, buf, buf_len)
#define hlog_warn(buf, buf_len) \
    hzlog(_hex_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, buf, buf_len)
#define hlog_notice(buf, buf_len) \
    hzlog(_hex_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, buf, buf_len)
#define hlog_info(buf, buf_len) \
    hzlog(_hex_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, buf, buf_len)
#define hlog_debug(buf, buf_len) \
    hzlog(_hex_category, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, buf, buf_len)


/* vdzlog macros */
#define vdlog_fatal(format, args) \
    vdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, format, args)
#define vdlog_error(format, args) \
    vdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, format, args)
#define vdlog_warn(format, args) \
    vdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, format, args)
#define vdlog_notice(format, args) \
    vdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, format, args)
#define vdlog_info(format, args) \
    vdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, format, args)
#define vdlog_debug(format, args) \
    vdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, format, args)

/* hdzlog macros */
#define hdlog_fatal(buf, buf_len) \
    hdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_FATAL, buf, buf_len)
#define hdlog_error(buf, buf_len) \
    hdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_ERROR, buf, buf_len)
#define hdlog_warn(buf, buf_len) \
    hdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_WARN, buf, buf_len)
#define hdlog_notice(buf, buf_len) \
    hdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_NOTICE, buf, buf_len)
#define hdlog_info(buf, buf_len) \
    hdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_INFO, buf, buf_len)
#define hdlog_debug(buf, buf_len) \
    hdzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
    ZLOG_LEVEL_DEBUG, buf, buf_len)

#ifdef __cplusplus
}
#endif

#endif // LOG_H
