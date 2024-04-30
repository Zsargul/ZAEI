#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <dirent.h>

#include "util/miscutils.h"

static int debugMode = 0;

/* Helper for _mkdir.
 * Return values:
 * 0: Directory was created successfully or already exists
 * 1: Directory could not be created
 */
static int mkdir_ifn_exists(const char* dir, mode_t mode) {
	struct stat st;
	errno = 0;

	if (mkdir(dir, mode) == 0)
		return 0;

	/* If it fails for any reason other than the directory already existing, return failure */
	if (errno != EEXIST)
		return 1;

	/* Check if the existing path exists */
	if (stat(dir, &st) != 0)
		return 1;

	/* If it does not, fail with ENOTDIR */
	if (!S_ISDIR(st.st_mode)) {
		errno = ENOTDIR;
		return 1;
	}

	errno = 0;
	return 0;
}

/* mkdir, but return successfully if the directory already exists..
 * Return values:
 * 0: Directory was created or exists already
 * 1: Failure creating the directory
 */
int _mkdir(const char* dirPath) {
	char *_path = NULL;
	char *p;
	int ret = 1;
	mode_t mode = 0755; /* rwx r-x r-x */

	errno = 0;

	/* Check if string is empty */
	if (strcmp(dirPath, "") == 0) {
		errno = ENOENT;
		return ret;
	}

	/* Make mutable string copy */
	_path = strdup(dirPath);
	if (_path == NULL) {
		free(_path);
		return ret;
	}

	/* Iterate string */
	for (p = _path + 1; *p; p++) {
		if (*p == '/') {
			*p = '\0'; /* Truncate temporarily */

			if (mkdir_ifn_exists(_path, mode) != 0) {
				free(_path);
				return ret;
			}
			*p = '/';
		}
	}

	if (mkdir_ifn_exists(_path, mode) != 0) {
		free(_path);
		return ret;
	}

	ret = 0;
	free(_path);
	return ret;
}

void enable_debug_mode() {
	debugMode = 1;
}

/* Wrapper for fprintf for debugging */
int dbg_fprintf(FILE *stream, const char *format, ...) {
	if (!debugMode)
		return 0;

	va_list arg;
	int done;

	va_start(arg, format);
	done = vfprintf(stream, format, arg);
	va_end(arg);

	return done;
}

/* Returns 0 if directory is not empty */
int dir_not_empty(const char* dirname) {
	int n = 0;
	struct dirent *d;

	DIR *dir = opendir(dirname); 
	if (dir == NULL) /* Not a directory or does not exist */
		return 1;

	while ((d = readdir(dir)) != NULL) {
		if (++n > 2)
			break;
	}
	closedir(dir);

	if (n <= 2) /* Empty */
		return 1;
	else
		return 0;
}

void err_usage(char* progName, char* errStr) {
	fprintf(stderr, "%s\n", errStr);
	usage(progName);
}

void usage(char* progName) {
	char *helpMsg =
		"  -h, --help			Print usage help and program information\n"
		"  -b, --debug			Enable debug mode, which gives more verbose logs\n"
		;

	fprintf(stdout, "Usage: %s\n\n%s", progName, helpMsg);
}
