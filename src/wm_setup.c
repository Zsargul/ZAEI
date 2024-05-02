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
	if (repo_exists(repoUrl) == 0) {
		fprintf(stdout, "Dwmblocks: specified git repository exists. Cloning...\n");
	} else {
		fprintf(stderr, "Dwmblocks: specified git repository with URL \"%s\" does not exist!\n", repoUrl);
		return 1;
	}

	if (clone_repo("Dwmblocks", repoUrl, targetDir) != 0)
		return 1;

	return 0;
}
