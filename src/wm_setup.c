#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "wm_setup.h"
#include "constants.h"
#include "util/miscutils.h"

int install_dwm(const char* repoUrl, const char* targetDir) {
	char fullPath[MAX_STR_LEN];

	/* Check if repo exists */
	if (repo_exists(repoUrl)) {
		fprintf(stdout, "Dwm: specified git repository exists. Cloning...\n");
	} else {
		fprintf(stdout, "Dwm: specified git repository with URL \"%s\" does not exist! Exiting.\n", repoUrl);
		exit(EXIT_FAILURE);
	}

	clone_repo("Dwm", repoUrl, targetDir);

	return 0;	
}

int install_dwmblocks(const char* repoUrl, const char* targetDir) {
	char fullPath[MAX_STR_LEN];

	/* Check if repo exists */
	if (repo_exists(repoUrl)) {
		fprintf(stdout, "Dwmblocks: specified git repository exists. Cloning...\n");
	} else {
		fprintf(stderr, "Dwmblocks: specified git repository with URL \"%s\" does not exist! Exiting.\n", repoUrl);
		exit(EXIT_FAILURE);
	}

	clone_repo("Dwmblocks", repoUrl, targetDir);

	return 0;
}

int clone_repo(const char* name, const char* repoUrl, const char* targetDir) {
	char command[MAX_STR_LEN];

	/* Check if git is installed */
	if (git_installed() != 0) {
		fprintf(stderr, "Git is not installed/configured correctly on your system. Please install git before running this tool with any git repositories specified in the config.\n");
		exit(EXIT_FAILURE);
	}

	/* Create specified directory if it does not exist */
	struct stat st;
	if (stat(targetDir, &st) != 0) {
		fprintf(stdout, "%s: Directory %s does not exist. Creating it...\n", name, targetDir);
		if (_mkdir(targetDir) != 0) {
			fprintf(stderr, "%s: Error creating directory %s. Exiting...\n", name, targetDir);
			exit(EXIT_FAILURE);
		}
	}

	/* Download repo */
	snprintf(command, sizeof(command), "git clone %s %s", repoUrl, targetDir);
	/* TODO: Change this to a fork()/exec() call */
	int ret = system(command);

	if (ret == 0) {
		fprintf(stdout, "%s: Repository successfully cloned.\n", name);
	} else {
		fprintf(stderr, "%s: Unable to clone repository. Exiting...\n", name);
		exit(EXIT_FAILURE);
	}

	return 0;
}

/* TODO: Change this to a fork()/exec() call */
int repo_exists(const char* repoUrl) {
	char cmd[MAX_STR_LEN];
	snprintf(cmd, sizeof(cmd), "git ls-remote %s", repoUrl);

	return system(cmd);
}
