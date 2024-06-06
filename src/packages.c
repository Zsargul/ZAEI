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

/* Arguments:
 * pkgsListFile - Name of .csv file containing list of packages
 *
 * Returns:
 * -1 - Error installing packages
 * successfulInstalls - Number of package which were successfully installed
 */
int install_packages(const char* pkgsListFile) {
	int packageCount = csv_lines(pkgsListFile);
	if (packageCount == -1)
		return -1;

	Package* pkgs = (Package*)malloc(packageCount * sizeof(Package));; /* Allocate memory for package array */

	if (parse_package_list(pkgsListFile, pkgs, packageCount) != 0)
		return -1;

	int successfulInstalls = 0;
	int officialPkgsCount = 0;
	int aurPkgsCount = 0;

	/* TODO: Create tidier print messages using return characters etc. */
	for (int i = 0; i < packageCount; i++) {
		log_msg(stdout, INFO, "Installing package [%d/%d]\n", i+1, packageCount);

		Package* pkgPtr = &pkgs[i];
		int returnCode = install_package(pkgPtr);
		
		switch (returnCode) {
			case 0:
				successfulInstalls++;
				(pkgPtr->onAur) ? aurPkgsCount++ : officialPkgsCount++;
				break;
			case 1: /* Error installing package */
				return -1;
				break;
			case 2: 
				/* Error installing package stemming from execvp returning. If this happens, the program must exit 
				 * here in order to prevent the child process from redundantly installing packages in parallel with
				 * the parent process. */
				exit(EXIT_FAILURE);
				break;
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

		return successfulInstalls;
	}
}

/* Arguments:
 * Package *pkg - Pointer to pacakge to install
 *
 * Returns:
 * 2 - Error caused by execvp returning
 * 1 - Error installing package
 * 0 - Success installing package
 */
int install_package(Package *pkg) {
	/* Determine if package is installed through AUR or official repos */
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

	int returnCode = 0;
	switch (forkPid) {
		case -1:
		   log_msg(stderr, ERR, "Error forking process while installing package '%s': %s\n", pkg->name, strerror(errno));	
		   returnCode = 1;
		   break;
		case 0: /* Child process */
		   const int execResult = execvp(cmdArgs[0], (char **)cmdArgs);

		   assert(execResult == -1); /* Exec only returns on errors */
		   log_msg(stderr, ERR, "Error installing package '%s': %s\n", pkg->name, strerror(errno));

		   returnCode = 2;
		   break;
		default: /* Parent process */
		   int childResult = -1;
		   const pid_t waitPid = wait(&childResult);

		   if (waitPid == -1) {
			   log_msg(stderr, ERR, "Error installing package '%s': Failed to wait for PID %d. %s\n", pkg->name, forkPid, strerror(errno));
			   returnCode = 1;
			   break;
		   }

		   /* Ensure that we waited for the correct process */
		   if (forkPid != waitPid) {
			   log_msg(stderr, ERR, "Error installing package '%s': PID of forked process and wait process are not the same.\n", pkg->name);
			   returnCode = 1;
		   }
		   break;
	}

	return returnCode;
}

