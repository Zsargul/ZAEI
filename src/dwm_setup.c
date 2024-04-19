#include <stdio.h>
#include <stdlib.h>

#include "dwm_setup.h"
#include "constants.h"

int setup_dwm() {
	return 0;	
}

int dwm_clone_git(const char* repoUrl, const char* targetDir) {
	char command[MAX_STR_LEN];

	/* Check if git is install here */

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
