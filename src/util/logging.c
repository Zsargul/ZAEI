#include <time.h>

#include "logging.h"

/* fprintf wrapper used for logging. */
void log(FILE* stream, const char* type, const char* format, ...) {	
	va_list arg;
	int done;

	int padLen = LOG_MSG_LEN - strlen(type);
	if (padLen < 0) padLen = 0;

	fprintf(stream, "[ ---%*s%*s--- ]", 10+strlen(type)/2, type, 10-strlen(s)/2,""); /* Print log message type */

	va_start(arg, format);
	done = vfprintf(stream, format, arg);
	va_end(arg);

	return done;
}

void usage(char* progName) {
	char *helpMsg =
		"  -h, --help                   Print usage help and program information\n"
		"  -b, --debug                  Enable debug mode, which gives more verbose logs\n"
		;

	fprintf(stdout, "Usage: %s\n\n%s", progName, helpMsg);
}