#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#include "constants.h"
#include "util/gitutils.h"
#include "util/miscutils.h"

/* TODO: Test the various possible output codes for the functions here, as some of the git utilities 
 * especially return false positives and false negatives */

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
	/* TODO: Output the possible error "targetDir already exists and is not empty" */
	int ret = 1;

	/* Check if git is installed */
	if (git_installed() != 0) {
		fprintf(stderr, "Git is not installed/configured correctly on your system. Please install git before running this tool with any git repositories specified in the config.\n");
		ret = 1;
	}

	/* Create specified directory if it does not exist */
	struct stat st;
	if (stat(targetDir, &st) != 0) {
		fprintf(stdout, "%s: Directory %s does not exist. Creating it...\n", name, targetDir);
		if (_mkdir(targetDir) != 0) {
			fprintf(stderr, "%s: Error creating directory %s.\n", name, targetDir);
			ret = 1;
		}
	}

	/* Download repo */
	const char* cmdArgs[] = {
		[0] = "git",
		[1] = "clone",
		[2] = repoUrl,
		[3] = targetDir,
		[4] = NULL,
	};

	const pid_t forkPid = fork();

	switch(forkPid) {
		case -1:
			const int forkErrno = errno;
			fprintf(stderr, "Could not fork new process: %s\n", strerror(forkErrno));
			ret = 1;
		case 0:
			/* Pipe stdout and stderr to /dev/null */
			int devnull = open("/dev/null", O_WRONLY);

			dup2(devnull, 1);
			close(1);
			dup2(devnull, 2);
			close(2);
			close(devnull);

			const int execResult = execvp(cmdArgs[0], (char **)cmdArgs);

			assert(execResult == -1);
			int const execErrno = errno;
			fprintf(stderr, "clone_repo(): Error from execvp() - %s\n", strerror(execErrno));

			ret = 1;
		default:
			int status;
			int returnedPid = waitpid(forkPid, &status, 0);
			if (returnedPid) { /* Child exited successfully */
				assert(forkPid == returnedPid);

				dbg_fprintf(stdout, "clone_repo(): Child exited with code %d\n", status);
				if (WIFEXITED(status) && !WEXITSTATUS(status)) { /* Repo was cloned successfully */
					dbg_fprintf(stdout, "repo_exists(): Git command failure\n", status);
					ret = 0;
				} else if (WIFEXITED(status) && WEXITSTATUS(status)) {
					dbg_fprintf(stdout, "clone_repo(): Git command success\n", status);
					ret = -1; /* Repo doesn't exist */
				}
			} else {
				int waitpidErrno = errno;
				fprintf(stderr, "Error waiting for PID %d: %s\n", waitpidErrno, strerror(waitpidErrno));
				ret = -1;
			}
	}
	return ret;
}
/* Return values:
 * -1: Error
 *  0: Repo exists
 *  1: Repo doesn't exist
 */
int repo_exists(const char* repoUrl) {
	int ret = -1;

	const char* cmdArgs[] = {
		[0] = "git",
		[1] = "ls-remote",
		[2] = repoUrl,
		NULL,
	};

	const pid_t forkPid = fork();

	switch (forkPid) {
		case -1:
			const int forkErrno = errno;
			fprintf(stderr, "Could not fork new process: %s\n", strerror(forkErrno));
			ret = -1;
		case 0:
			/* Pipe stdout and stderr to /dev/null */
			int devnull = open("/dev/null", O_WRONLY);

			dup2(devnull, 1);
			close(1);
			dup2(devnull, 2);
			close(2);
			close(devnull);

			const int execResult = execvp(cmdArgs[0], (char **)cmdArgs);

			assert(execResult == -1);
			int const execErrno = errno;
			fprintf(stderr, "repo_exists(): Error from execvp() - %s\n", strerror(execErrno));

			ret = -1;
		default:
			int status;
			int returnedPid = waitpid(forkPid, &status, 0);
			if (returnedPid) { /* Child exited successfully */
				assert(forkPid == returnedPid);

				dbg_fprintf(stdout, "repo_exists(): Child exited with code %d\n", status);
				if (WIFEXITED(status) && !WEXITSTATUS(status)) { /* Command returned successfully meaning repo exists */
					dbg_fprintf(stdout, "repo_exists(): Git command failure\n", status);
					ret = 0;
				} else if (WIFEXITED(status) && WEXITSTATUS(status)) {
					dbg_fprintf(stdout, "repo_exists(): Git command success\n", status);
					ret = 1; /* Repo doesn't exist */
				}
			} else {
				int waitpidErrno = errno;
				fprintf(stderr, "Error waiting for PID %d: %s\n", waitpidErrno, strerror(waitpidErrno));
				ret = -1;
			}
	}
	return ret;
}
