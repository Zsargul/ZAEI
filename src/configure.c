#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "constants.h"
#include "util/miscutils.h"
#include "configure.h"

int init_config(int argc, char **argv, Config* config) {
	config_t cfg;
	config_init(&cfg);

	if (config_read_file(&cfg, CONFIG_FILE) != CONFIG_TRUE) {
		fprintf(stderr, "Libconfig error: %s:%d - %s\n", config_error_file(&cfg), 
				config_error_text(&cfg), config_error_text(&cfg));
		return -1;
	}

	/* Get AUR helper */
	if (config_lookup_string(&cfg, "aur_helper", &config->aur_helper)) {
		dbg_fprintf(stdout, "Config - Found aur_helper: %s\n", config->aur_helper);
	} else {
		fprintf(stderr, "Unable to find 'aur_helper' value in %s!\n", CONFIG_FILE);
		return -1;
	}
	
	/* Get packages .csv file */
	if (config_lookup_string(&cfg, "packages_file", &config->packages_file)) { 
		dbg_fprintf(stdout, "Config - Found packages_file: %s\n", &config->packages_file);
	} else {
		fprintf(stderr, "Unable to find 'packages_file' value in %s!\n", CONFIG_FILE);
		return -1;
	}

	/* Get DWM git URL */
	if (config_lookup_string(&cfg, "dwm_git_url", &config->dwm_git_url)) {
		dgb_fprintf(stdout, "Config - Found dwm_git_url: %s\n", &config->dwm_git_url);
	} else {
		fprintf(stderr, "Unable to find 'dwm_git_url' value in %s!\n", CONFIG_FILE);
		return -1;
	}

	

	
}

/* Parse any command line options. */
int opt_handler(int argc, char **argv) {
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
