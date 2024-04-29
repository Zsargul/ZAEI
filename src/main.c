#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "util/miscutils.h"
#include "configure.h"

/* TODO: Include libonfig system path in makefile instead of just copying the header into include/. Same
 * goes for .a library */
int main(int argc, char **argv) {
	Config* config = (Config*)malloc(sizeof(Config));

	if (init_config(config) != 0) {
		free(config);
		fprintf(stderr, "Exiting...\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

