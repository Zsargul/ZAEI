#ifndef UTIL_H
#define UTIL_H

int _mkdir(const char* dir);
void enable_debug_mode();
int dbg_fprintf(FILE *stream, const char *format, ...);

#endif
