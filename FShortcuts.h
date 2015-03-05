#ifndef __FSHORTCUTS_H__
#define __FSHORTCUTS_H__

#include <stdlib.h>

static void _fc_cleanupWithFree(void *ptr) { free(*(void **)ptr); }
#define AutoFree __attribute((cleanup(_fc_cleanupWithFree)))

#endif
