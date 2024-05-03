#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <dirent.h>

#include <libconfig.h>

#include "constants.h"
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

char* libconfig_version() {
	char* str = malloc(MAX_STR_LEN);
	if (snprintf(str, MAX_STR_LEN, "%d.%d.%d", LIBCONFIG_VER_MAJOR,
			LIBCONFIG_VER_MINOR, LIBCONFIG_VER_REVISION) < 0) {
		return NULL;
	}

	return str;
}
