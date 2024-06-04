#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "constants.h"
#include "util/logging.h"

/* fprintf wrapper used for logging. */
void log_msg(FILE* stream, const char* type, const char* format, ...) {	
	va_list arg;

	int padLen, padLeft, padRight;
	padLen = (LOG_MSG_LEN - strlen(type));

	if (padLen < 0) {
		padLeft = padRight = 0;
	} else {
		padLeft = padRight = (padLen / 2);
		if (padLen % 2 != 0) padLeft++; /* Add extra padding to account for odd number length strings */
	}

	fprintf(stream, "[ %*s%s%*s ] ", padLeft, "", type, padRight, ""); /* Print log message type */

	va_start(arg, format);
	vfprintf(stream, format, arg);
	va_end(arg);
}

void usage(char* progName) {
	char *helpMsg =
		"  -h, --help                   Print usage help and program information\n"
		"  -b, --debug                  Enable debug mode, which gives more verbose logs\n"
		;

	fprintf(stdout, "Usage: %s\n\n%s", progName, helpMsg);
}

void failure() {
	fprintf(stdout, "Fatal error encountered. Exiting.\n");
	exit(EXIT_FAILURE);
}
