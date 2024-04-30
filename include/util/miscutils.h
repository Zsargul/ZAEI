#ifndef MISCUTILS_H
#define MISCUTILS_H

int _mkdir(const char* dir);
void enable_debug_mode();
int dbg_fprintf(FILE *stream, const char *format, ...);
int git_installed();
int dir_not_empty(const char* dirname);
void err_usage(char* progName, char* errStr);
void usage(char* progName);

#endif
