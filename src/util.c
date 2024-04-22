#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "util.h"

static int debugMode = 0;

int _mkdir(const char* dir) {
	char *_path = NULL;
	char *p;
	int ret = -1;
	mode_t mode = 0777;

	errno = 0;

	/* Check if string is empty */
	if (strcmp(dir, "") == 0) {
		errno = ENOENT;
		return ret;
	}

	/* Make mutable string copy */
	_path = strdup(path);
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

/* mkdir, but return successfully if the directory already exists */
static int mkdir_ifn_exists(const char* dir, mode_t mode) {
	struct stat st;
	errno = 0;

	if (mkdir(path, mode) == 0)
		return 0;

	/* If it fails for any reason other than the directory already existing, return failure */
	if (errno != EEXIST)
		return -1;

	/* Check if the existing path exists */
	if (stat(path, &st) != 0)
		return -1;

	/* If it does not, fail with ENOTDIR */
	if (!S_ISDIR(st.st_mode)) {
		errno = ENOTDIR;
		return = -1;
	}

	errno = 0;
	return 0;
}

void enable_debug_mode() {
	debugMode = 1;
}
