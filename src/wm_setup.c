#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "wm_setup.h"
#include "constants.h"
#include "util/miscutils.h"

/* Return values:
 * 0: Success
 * 1: Critical error
 */
int install_dwm(const char* repoUrl, const char* targetDir) {
	/* Check if repo exists */
	if (repo_exists(repoUrl)) {
		fprintf(stdout, "Dwm: specified git repository exists. Cloning...\n");
	} else {
		fprintf(stdout, "Dwm: specified git repository with URL \"%s\" does not exist! Exiting.\n", repoUrl);
		return 1;
	}

	if (clone_repo("Dwm", repoUrl, targetDir) != 0) 
		return 1;

	/* TODO: Get name of git repo and use it to double check that it is not empty using dir_not_empty */

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
		fprintf(stderr, "Dwmblocks: specified git repository with URL \"%s\" does not exist! Exiting.\n", repoUrl);
		return 1;
	}

	if (clone_repo("Dwmblocks", repoUrl, targetDir) != 0)
		return 1;

	/* TODO: Get name of git repo and use it to double check that it is not empty using dir_not_empty */

	return 0;
}

/* Return values:
 * 0: Success
 * 1: Critical error
 */
int clone_repo(const char* name, const char* repoUrl, const char* targetDir) {
	/* Check if git is installed */
	if (git_installed() != 0) {
		fprintf(stderr, "Git is not installed/configured correctly on your system. Please install git before running this tool with any git repositories specified in the config.\n");
		return 1;
	}

	/* Create specified directory if it does not exist */
	struct stat st;
	if (stat(targetDir, &st) != 0) {
		fprintf(stdout, "%s: Directory %s does not exist. Creating it...\n", name, targetDir);
		if (_mkdir(targetDir) != 0) {
			fprintf(stderr, "%s: Error creating directory %s. Exiting...\n", name, targetDir);
			return 1;
		}
	}

	/* Download repo */
	char command[MAX_STR_LEN];
	snprintf(command, sizeof(command), "git clone %s %s", repoUrl, targetDir);
	/* TODO: Change this to a fork()/exec() call */
	int ret = system(command);

	if (ret == 0) {
		fprintf(stdout, "%s: Repository successfully cloned.\n", name);
	} else {
		fprintf(stderr, "%s: Unable to clone repository. Exiting...\n", name);
		return 1;
	}

	return 0;
}

/* TODO: Change this to a fork()/exec() call */
/* Returns 0 if repo doesn't exist. */
int repo_exists(const char* repoUrl) {
	char cmd[MAX_STR_LEN];
	snprintf(cmd, sizeof(cmd), "git ls-remote %s", repoUrl);

	return system(cmd);
}
