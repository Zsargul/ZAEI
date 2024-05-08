#include <stdio.h>
#include <stdlib.h>

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
	if (repo_exists(repoUrl) == 0) {
		if (clone_repo(repoUrl, targetDir) != 0) {
			log_msg(stderr, ERR, "Unable to clone repository %s.\n", repoUrl);
			failure();
		}
		log_msg(stdout, INFO, "Repository at %s cloned successfully into %s.\n", repoUrl, targetDir);
	} else {
		log_msg(stderr, ERR, "Repository at %s does not exist.\n", repoUrl);
		failure();
	}


	if (clone_repo(repoUrl, targetDir) != 0) 
		return 1;

	return 0;	
}

/* Return values:
 * 0: Success
 * 1: Critical error
 */
int install_dwmblocks(const char* repoUrl, const char* targetDir) {
	/* Check if repo exists */
	int exists = repo_exists(repoUrl);

	switch(exists) {
		case -1:
			fprintf(stderr, "Error in fork/exec when trying to clone git repository.\n");
			return 1;
		case 1:
			fprintf(stderr, "Specified repository cannot be accessed.\n");
			return 1;
		default: /* 0 - Repository exists */
			fprintf(stdout, "Dwmblocks: git repository %s exists. Cloning...\n", repoUrl);

			int clone = clone_repo(repoUrl, targetDir);

			/* TODO: clone dwm here */
	}

	return 0;
}
