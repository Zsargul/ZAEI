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

/* Return values:
 * -1: Error
 *  0: Repo exists
 *  1: Repo doesn't exist
 */
int repo_exists(const char* repoUrl) {
	char cmd[MAX_STR_LEN];
	const char* cmdArgs[] = {
		[0] = "ls-config",
		[1] = repoUrl,
		NULL,
	};

	const pid_t forkPid = fork();

	switch (forkPid) {
		case -1:
			const int forkErrno = errno;
			fprintf(stderr, "Could not fork new process: %s\n", strerror(forkErrno));
			return -1;
		case 0:
			strcpy(cmd, "git");
			const int execResult = execvp(cmd, (char **)cmdArgs);

			assert(execResult == -1);
			int const execErrno = errno;
			fprintf("repo_exists(): Error from execvp() - %s\n", strerror(execErrno));

			return -1;
		default:
			int status;
			if (waitpid(forkPid, &status, 0) > 0) { /* Child exited successfully */
					assert(forkPid == waitResult);

					dbg_fprintf(stdout, "repo_exists(): Child exited with code %d", status);
					if (WIFEXITED(status) && !WEXITSTATUS(status)) { /* Command returned successfully meaning repo exists */
							return 0;
					} else if (WIFEXITED(status) && WEXITSTATUS(status)) {
							return 1; /* Repo doesn't exist */
					}
			} else {
					int waitpidErrno = errno;
					fprintf(stderr, "Error waiting for PID %d: %s\n", waitpidErrno, strerror(waitpidErrno));
					return -1;
			}
	}
}
