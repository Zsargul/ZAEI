#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/wait.h>

#include "packages.h"
#include "util/parsecsv.h"

/* TODO: Stick to one naming convention for variables for christ's sake */
int install_packages() {
	int packageCount = csv_lines(PACKAGES_LIST_FILE);
	if (packageCount == -1)
		exit(EXIT_FAILURE);

	Package pkgs[packageCount]; /* Array of packages to install */

	if (packageCount != (sizeof(pkgs) / sizeof(pkgs[0]))) {
		fprintf(stderr, "Error creating pkgs array. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	if (parse_package_list(PACKAGES_LIST_FILE, pkgs, packageCount) == -1)
		exit(EXIT_FAILURE);

	int successfulInstalls = 0;
	int officialPkgsCount = 0;
	int aurPkgsCount = 0;

	/* TODO: Create tidier print messages using return characters etc. */
	for (size_t i = 0; i < packageCount; i++) {
		Package* pkgPtr = &pkgs[i];

		int ret = install_package(pkgPtr);
		
		fprintf(stdout, "[%ld/%d]", i+1, packageCount);
		if (ret != 0 && pkgPtr->req) {
			fprintf(stderr, "%ld: Package %s, was not installed successfully, but it is not required.\n", i, *(pkgs[i].name));
		} else if (ret != 0 && !(pkgPtr->req)) {
			fprintf(stderr, "%ld: REQUIRED Package %s, was not installed successfully. Exiting.\n", i, *(pkgs[i].name));
			exit(EXIT_FAILURE);
		} else {
			fprintf(stdout, "%ld: %s installed successfully\n", i, *(pkgs[i].name));

			successfulInstalls++;
			(pkgPtr->onAur) ? aurPkgsCount++ : officialPkgsCount++;
		}
	}

	fprintf(stdout,
			"Successfully installed %d/%d packages.\n"
			"Packages from official repositories: %d\n"
			"Packages from Arch user repository: %d\n",
			successfulInstalls,
			packageCount,
			officialPkgsCount,
			aurPkgsCount);



	return 0;
}

int install_package(Package *pkg) {
	char cmd[MAX_STR_LEN];
	const char *cmd_args[] = { 
		[0] = "--noconfirm", 
		[1] = "--needed",
		[2] = "-S",
		[3] = NULL,
	};

	const pid_t forkPid = fork();

	/* TODO: Rethink return values for this? */
	switch (forkPid) {
		case -1:
			const int forkErrno = errno; /* Save error code */
			fprintf(stderr, "Could not fork new process: %s\n", strerror(forkErrno));
			return forkErrno;
		case 0: /* Child process */
			fprintf(stdout, "Child process executing: %s...\n", cmd);

			/* Use AUR helper or official repos for installation */
			strcpy(cmd, pkg->onAur ? AUR_HELPER : "pacman");

			const int execResult = execvp(cmd, (char **)cmd_args);

			assert(execResult == -1); /* Exec only returns on errors */
			int const execErrno = errno;
			fprintf(stderr, "Failure using execvp() with package installation command: %s\n", strerror(execErrno));

			return execErrno;
		default:
			fprintf(stdout, "Forking process and waiting for PID %d\n", forkPid);
			int childResult = -1;
			const pid_t waitResult = wait(&childResult);

			if (waitResult == -1) {
				const int forkErrno = errno;
				fprintf(stderr, "Could not wait for PID %d: %s\n", forkPid, strerror(forkErrno));

				return forkErrno;
			}

			/* Assert that what we forked is what we waited for */
			assert(forkPid == waitResult);
			fprintf(stdout, "Child process with PID %d exited with a status of %d\n", forkPid, childResult);
			break;
	}
	return 0;
}
