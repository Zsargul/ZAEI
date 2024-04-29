#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "util/miscutils.h"
#include "configure.h"

Config config;

/* Used to parse arguments and other configurations. Essentially just
 * opt handling but off-loaded from main. */
int init_config_old(int argc, char **argv) {
	char* name = argv[0];
	int opt;
	
	/* Argument flags */
	int c_flag = 0;
	int d_flag = 0;
	int h_flag = 0;
	int dbg_flag = 0;

	/* Possible short arguments */
	const char *shortOptions = "dh";

	/* TODO: Debug and check that ALL of these work, short and long options */
	struct option longOptions[] = {
		{"help", 	no_argument, 		&h_flag, 	'h'},
		{"debug",	no_argument,		&dbg_flag,	'd'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, shortOptions, longOptions, NULL)) != -1) {
		switch (opt) {
			case 'h':
				h_flag = 1;
				usage(name);
				exit(EXIT_SUCCESS);
				break;
			case 'd':
				dbg_flag = 1;
				enable_debug_mode();
				dbg_fprintf(stdout, "! Enabled debug messages !\n");
				break;
			case '?': /* Invalid option or missing argument */
				err_usage(name, "Unrecognised argument\n");
				exit(EXIT_FAILURE);
		}
	}

	/* Check for required arguments */
	if (!c_flag) {
		err_usage(name, "Path to .csv package list is required\n");
		exit(EXIT_FAILURE);
	}

	if (!d_flag) {
		err_usage(name, "URL to dwm repo is required\n");
		exit(EXIT_FAILURE);
	}

	for (int i = optind; i < argc; ++i) {
		/* Skip over any non-option arguments */
	}

	return 0;
}

int read_config_file() {
	config_t cfg;
	config_init(&cfg);
	return 0;	
}
