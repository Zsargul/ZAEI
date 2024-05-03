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
#include "util/logging.h"

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
 *  0: Success
 *  1: Repository could not be cloned
 */
int clone_repo(const char* repoUrl, const char* targetDir) {
	/* TODO: Output the possible error "targetDir already exists and is not empty" */

	/* Check if git is installed */
	if (git_installed() != 0) {
		log_msg(stderr, ERR, "Git is not installed/configured correctly on your system. Please install git before running this tool with any git repositories specified in the config.\n");
		failure();
	}

	/* Create specified directory if it does not exist */
	struct stat st;
	if (stat(targetDir, &st) != 0) {
		log_msg(stdout, INFO, "clone_repo(): Directory %s does not exist. Creating it...\n", targetDir);
		if (_mkdir(targetDir) != 0) {
			log_msg(stderr, ERR, "clone_repo(): Error creating directory %s: %s.\n", targetDir);
			failure();
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
			log_msg(stderr, ERR, "clone_repo(): could not fork new process: %s\n", strerror(forkErrno));
			failure();
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
			log_msg(stderr, ERR, "clone_repo(): Error from execvp() - %s\n", strerror(execErrno));

			failure();
		default:
			int status;
			int returnedPid = waitpid(forkPid, &status, 0);
			if (returnedPid) { /* Child exited successfully */
				assert(forkPid == returnedPid);

				if (WIFEXITED(status) && !WEXITSTATUS(status)) { /* Repo was cloned successfully */
					log_msg(stdout, INFO, "Successfully cloned %s to %s\n", repoUrl, targetDir); /* TODO: Get just last part of the url i.e repo name and print that instead. dont pass it as a arg to the function. */
					return 0;
				} else if (WIFEXITED(status) && WEXITSTATUS(status)) {
					log_msg(stdout, ERR, "clone_repo(): Git command failure\n", status);
					return 1;
				}
			} else {
				int waitpidErrno = errno;
				log_msg(stderr, ERR, "clone_repo(): Error waiting for PID %d: %s\n", waitpidErrno, strerror(waitpidErrno));
				failure();
			}
	}
	return 0;
}

/* Return values:
 *  0: Repo exists
 *  1: Repo doesn't exist
 */
int repo_exists(const char* repoUrl) {
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
			log_msg(stderr, ERR, "repo_exists(): could not fork new process: %s\n", strerror(forkErrno));
			failure();
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
			log_msg(stderr, ERR, "repo_exists(): Error from execvp() - %s\n", strerror(execErrno));

			failure();
		default:
			int status;
			int returnedPid = waitpid(forkPid, &status, 0);
			if (returnedPid) { /* Child exited successfully */
				assert(forkPid == returnedPid);

				dbg_fprintf(stdout, "repo_exists(): Child exited with code %d\n", status);
				if (WIFEXITED(status) && !WEXITSTATUS(status)) { /* Command returned successfully meaning repo exists */
					log_msg(stdout, INFO, "Repository at %s exists.\n", repoUrl);
					return 0;
				} else if (WIFEXITED(status) && WEXITSTATUS(status)) {
					log_msg(stdout, ERR, "repo_exists(): Repository at %s could not be found.\n", repoUrl);
					return 1;
				}
			} else {
				int waitpidErrno = errno;
				log_msg(stderr, ERR, "repo_exists(): Error waiting for PID %d: %s\n", waitpidErrno, strerror(waitpidErrno));
				failure();
			}
	}
	return 0;
}
