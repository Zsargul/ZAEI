#ifndef UTIL_H
#define UTIL_H

#include <sys/stat.h>

int _mkdir(const char* dir);
static int mkdir_ifn_exists(const char *dir, mode_t mode);
void enable_debug_mode();

#endif
