#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/wait.h>

#include "packages.h"
#include "parsecsv.h"

int install_packages() {
	int no_of_packages = csv_lines(PACKAGES_LIST_FILE);
	if (no_of_packages == -1)
		exit(EXIT_FAILURE);

	Package pkgs[no_of_packages]; /* Array of packages to install */

	if (no_of_packages != (sizeof(pkgs) / sizeof(pkgs[0]))) {
		fprintf(stderr, "Error creating pkgs array. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	if (parse_package_list(PACKAGES_LIST_FILE, pkgs, no_of_packages) == -1)
		exit(EXIT_FAILURE);

	for (size_t i = 0; i < no_of_packages; i++) {
		Package* pkgPtr = &pkgs[i];

		int ret = install_package(pkgPtr);
		
		if (ret != 0 && pkgPtr->req) {
			fprintf(stderr, "%zd: Package %s, was not installed successfully, but it is not required.\n", i, *(pkgs[i].name));
		} else if (ret != 0 && !(pkgPtr->req)) {
			fprintf(stderr, "%zd: REQUIRED Package %s, was not installed successfully. Exiting.\n", i, *(pkgs[i].name));
			exit(EXIT_FAILURE);
		} else {
			fprintf(stdout, "%zd: %s installed successfully\n", i, *(pkgs[i].name));
		}
	}


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

	switch (forkPid) {
		case -1:
			const int forkErrno = errno; /* Save error code */
			fprintf(stderr, "Could not fork new process: %s\n",
					strerror(forkErrno));
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
