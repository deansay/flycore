#ifndef FLYCORE_H
#define FLYCORE_H

#include <sys/errno.h>

#include "flycore_global.h"
#include "libipc.h"
#ifdef __cplusplus
extern "C" {
#endif

void flymsleep(unsigned long msec);
void flyusleep(unsigned long usec);


#ifdef __cplusplus
}
#endif

#endif // FLYCORE_H
