#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "constants.h"
#include "util/miscutils.h"
#include "configure.h"
#include "packages.h"
#include "wm_setup.h"

/* TODO: Refactor so that most functions, except ones in main and maybe wm_setup, don't print any output, but rather
 * just return values. These values should then be used to print appropriate info/errors in main, rather than having
 * them randomly littered all over helper functions */

/* TODO: Header file for libconfig is in /usr/include. This is included in the makefile.
 * Now all thats left is statically linking libconfig.a without having to keep the .a file in
 * the local project directory. find out how to do this. */
int main(int argc, char **argv) {
	opt_handler(argc, argv);

	char* libconfigVer = libconfig_version();
	if (libconfigVer == NULL) {
		fprintf(stderr, "%s: Unable to get Libconfig version!\n", CRIT);
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "Using Libconfig version: %s\n", libconfigVer);
		free(libconfigVer);
	}

	Config* config = (Config*)malloc(sizeof(Config));

	if (init_config(config) != 0) {
		fprintf(stderr, "Exiting.\n");
		exit(EXIT_FAILURE);
	}

	/* Install all packages first */
	if (install_packages(config->packages_csv_path) == -1) {
		fprintf(stderr, "Exiting.\n");
		exit(EXIT_FAILURE);
	}


	/* TODO this is an optional config element. check if it exists before setting it up */
	/* Setup DWM */
	if (install_dwm(config->dwm_git_url, config->dwm_dir) != 0)  {
		fprintf(stderr, "Exiting.\n");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

