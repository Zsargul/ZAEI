#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

void log_msg(FILE* stream, const char* type, const char* format, ...);
void usage(char* progName);
void failure();

#endif
