#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/wait.h>

#include "packages.h"
#include "util/parsecsv.h"
#include "util/miscutils.h"
#include "util/logging.h"

/* TODO: Stick to one naming convention for variables for christ's sake */
/* Return values:
 * -1: Critical error
 *  Other: Number of installed packages
 */
int install_packages(const char* pkgsListFile) {
	int packageCount = csv_lines(pkgsListFile);
	if (packageCount == -1)
		return -1;

	Package* pkgs = (Package*)malloc(packageCount * sizeof(Package));; /* Array of packages to install */

	if (parse_package_list(pkgsListFile, pkgs, packageCount) != 0)
		return -1;

	int successfulInstalls = 0;
	int officialPkgsCount = 0;
	int aurPkgsCount = 0;

	/* TODO: Create tidier print messages using return characters etc. */
	for (int i = 0; i < packageCount; i++) {
		Package* pkgPtr = &pkgs[i];

		int ret = install_package(pkgPtr);
		
		log_msg(stdout, INFO, "Installing package [%d/%d]\n", i+1, packageCount);
		if (ret == 0) {
			successfulInstalls++;
			(pkgPtr->onAur) ? aurPkgsCount++ : officialPkgsCount++;
		}
	}

	/* TODO: Make sure this is tabbed */
	log_msg(stdout, INFO,
			"Successfully installed %d/%d packages.\n"
			"Packages from official repositories: %d\n"
			"Packages from Arch user repository: %d\n",
			successfulInstalls,
			packageCount,
			officialPkgsCount,
			aurPkgsCount);

	exit(1);
	return successfulInstalls;
}

int install_package(Package *pkg) {
	/* Use AUR helper or official repos for installation */
	/* TODO: Use config->aurhelper instead of AUR_HELPER */
	char packageManager[MAX_STR_LEN];
	strcpy(packageManager, pkg->onAur ? AUR_HELPER : "pacman");

	const char* cmdArgs[] = { 
		[0] = packageManager,
		[1] = "--noconfirm", 
		[2] = "--needed",
		[3] = "-S",
		[4] = NULL,
	};

	const pid_t forkPid = fork();

	/* TODO: Rethink return values for this? */
	switch (forkPid) {
		case -1:
			log_msg(stderr, ERR, "install_package(): could not fork new process: %s\n", strerror(errno));
			return 1;
		case 0: /* Child process */
			const int execResult = execvp(cmdArgs[0], (char **)cmdArgs);

			assert(execResult == -1); /* Exec only returns on errors */
			log_msg(stderr, ERR, "Error installing package '%s': %s\n", *(pkg->name), strerror(errno));

			if (pkg->req) {
				log_msg(stderr, ERR, "Failure to install required package '%s'.\n", *(pkg->name));
				/* TODO: Refactor this stuff so that failure to install a required package terminates the program. */
			}

			/* If execvp() called by the child process failed, the program should exit here otherwise the child process will return to the calling function, install_packages(), and continue to redundandtly install packages in parallel with the parent process. */
			exit(EXIT_FAILURE); 
		default:
			int childResult = -1;
			const pid_t waitResult = wait(&childResult);

			if (waitResult == -1) {
				log_msg(stderr, ERR, "install_package(): could not wait for PID %d: %s\n", forkPid, strerror(errno));
				return 1;
			}

			/* Assert that what we forked is what we waited for */
			assert(forkPid == waitResult);

			return 0;
	}
}
