#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "constants.h"
#include "util/gitutils.h"
#include "util/miscutils.h"

/* TODO: Change system() to fork()/exec() */
/* Returns 0 if git is installed. */
int git_installed() {
	if (system("git --version") != 0)
 		return 1;
 
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
                        fprintf(stderr, "%s: Error creating directory %s.\n", name, targetDir);
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
                fprintf(stderr, "%s: Unable to clone repository.\n", name);
                return 1;
        }

        return 0;
}
