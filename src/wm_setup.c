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
