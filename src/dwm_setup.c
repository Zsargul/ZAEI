#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "dwm_setup.h"
#include "constants.h"
#include "util.h"

int setup_dwm(const char* repoUrl) {
	char fullPath[MAX_STR_LEN];
	const char* configPath = ".config";
	const char* dwmPath = "suckless";
	const char* home;

	/* Check if repo exists */
	if (repo_exists(repoUrl)) {
		fprintf(stdout, "Specified Git repository exists. Cloning...\n");
	} else {
		fprintf(stdout, "Specified Get repository with URL \"%s\" does not exist! Exiting.\n", repoUrl);
		exit(EXIT_FAILURE);
	}

	/* Concatenate the full destination of the dwm directory. Maybe later let this be an argument rather
	 * than being hard-coded */
	home = getenv("HOME");
	snprintf(fullPath, sizeof(fullPath), "%s/%s/%s/", home, configPath, dwmPath);

	dwm_clone_git(repoUrl, fullPath);

	return 0;	
}

int dwm_clone_git(const char* repoUrl, const char* targetDir) {
	char command[MAX_STR_LEN];

	/* Check if git is installed here */

	/* Create specified directory if it does not exist */
	struct stat st;
	if (stat(targetDir, &st) != 0) {
		fprintf(stdout, "Directory %s does not exist. Creating it...\n", targetDir);
		if (_mkdir(targetDir) != 0) {
			fprintf(stderr, "Error creating directory %s. Exiting...\n", targetDir);
			exit(EXIT_FAILURE);
		}
	}

	/* Download repo */
	snprintf(command, sizeof(command), "git clone %s %s", repoUrl, targetDir);
	int ret = system(command);

	if (ret == 0) {
		fprintf(stdout, "DWM repository successfully cloned\n");
	} else {
		fprintf(stderr, "Unable to clone DWM repository. Exiting\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int repo_exists(const char* repoUrl) {
	char cmd[MAX_STR_LEN];
	snprintf(cmd, sizeof(cmd), "git ls-remote %s", repoUrl);

	return system(cmd);
}
