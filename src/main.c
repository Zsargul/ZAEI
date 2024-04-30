#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "util/miscutils.h"
#include "configure.h"
#include "packages.h"
#include "wm_setup.h"

/* TODO: Refactor so that most functions, except ones in main and maybe wm_setup, don't print any output, but rather
 * just return values. These values should then be used to print appropriate info/errors in main, rather than having
 * them randomly littered all over helper functions */

/* TODO: Include libonfig system path in makefile instead of just copying the header into include/. Same
 * goes for .a library */
int main(int argc, char **argv) {
	opt_handler(argc, argv);

	Config* config = (Config*)malloc(sizeof(Config));

	if (init_config(config) != 0) {
		free(config);
		fprintf(stderr, "Exiting.\n");
		return EXIT_FAILURE;
	}

	/* Install all packages first */
	if (install_packages(config->packages_csv_path) == -1) {
		exit(EXIT_FAILURE);


	/* TODO this is an optional config element. check if it exists before setting it up */
	/* Setup DWM */
	install_dwm(config->dwm_git_url, config->dwm_dir);


	return EXIT_SUCCESS;
}

