#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "wm_setup.h"
#include "constants.h"
#include "util/miscutils.h"
#include "util/gitutils.h"

/* Return values:
 * 0: Success
 * 1: Critical error
 */
int install_dwm(const char* repoUrl, const char* targetDir) {
	/* Check if repo exists */
	if (repo_exists(repoUrl)) {
		fprintf(stdout, "Dwm: specified git repository exists. Cloning...\n");
	} else {
		fprintf(stdout, "Dwm: specified git repository with URL \"%s\" does not exist!\n", repoUrl);
		return 1;
	}

	if (clone_repo("Dwm", repoUrl, targetDir) != 0) 
		return 1;

	return 0;	
}

/* Return values:
 * 0: Success
 * 1: Critical error
 */
int install_dwmblocks(const char* repoUrl, const char* targetDir) {
	/* Check if repo exists */
	if (repo_exists(repoUrl)) {
		fprintf(stdout, "Dwmblocks: specified git repository exists. Cloning...\n");
	} else {
		fprintf(stderr, "Dwmblocks: specified git repository with URL \"%s\" does not exist!\n", repoUrl);
		return 1;
	}

	if (clone_repo("Dwmblocks", repoUrl, targetDir) != 0)
		return 1;

	return 0;
}

/* TODO: Change this to a fork()/exec() call */
/* Returns 0 if repo doesn't exist. */
int repo_exists(const char* repoUrl) {
	char cmd[MAX_STR_LEN];
	snprintf(cmd, sizeof(cmd), "git ls-remote %s", repoUrl);

	return system(cmd);
}
