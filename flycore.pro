#-------------------------------------------------
#
# Project created by QtCreator 2017-08-04T23:12:20
#
#-------------------------------------------------

QT       -= core gui

TARGET = flycore
TEMPLATE = lib

CONFIG += qt plugin c++11

DEFINES += FLYCORE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    flycore.c \
    list.c \
    msgq.c \
    shm.c \
    ini.c \
    parson.c \
    unqlite.c \
    zlog/buf.c \
    zlog/category.c \
    zlog/category_table.c \
    zlog/conf.c \
    zlog/event.c \
    zlog/format.c \
    zlog/level.c \
    zlog/level_list.c \
    zlog/mdc.c \
    zlog/record.c \
    zlog/record_table.c \
    zlog/rotater.c \
    zlog/rule.c \
    zlog/spec.c \
    zlog/thread.c \
    zlog/zc_arraylist.c \
    zlog/zc_hashtable.c \
    zlog/zc_profile.c \
    zlog/zc_util.c \
    zlog/zlog-chk-conf.c \
    zlog/zlog.c \
    log.c \
    db.c \
    ipc.c \
    debug.c \
    dlmod.c \
    hash.c \
    zip.c \
    queue.c \
    stack.c \
    gerror.c \
    timer.c \
    state.c

HEADERS += \
    flycore.h \
    flycore_global.h \
    ipc_private.h \
    libipc.h \
    list.h \
    msgq.h \
    shm.h \
    ini.h \
    parson.h \
    unqlite.h \
    zlog.h \
    zlog/buf.h \
    zlog/category.h \
    zlog/category_table.h \
    zlog/conf.h \
    zlog/event.h \
    zlog/fmacros.h \
    zlog/format.h \
    zlog/level.h \
    zlog/level_list.h \
    zlog/mdc.h \
    zlog/record.h \
    zlog/record_table.h \
    zlog/rotater.h \
    zlog/rule.h \
    zlog/spec.h \
    zlog/thread.h \
    zlog/version.h \
    zlog/zc_arraylist.h \
    zlog/zc_defs.h \
    zlog/zc_hashtable.h \
    zlog/zc_profile.h \
    zlog/zc_util.h \
    zlog/zc_xplatform.h \
    zlog/zlog.h \
    log.h \
    db.h \
    dlmod.h \
    hash.h \
    zcore.h \
    zip.h \
    queue.h \
    stack.h \
    gerror.h \
    timer.h \
    state.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $${PWD}/../output/
