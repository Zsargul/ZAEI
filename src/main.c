#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "util.h"

static void err(char* progName, char* errStr);
static void usage(char* progName);

int main(int argc, char **argv) {
	char* name = argv[0];
	int opt;
	
	/* Argument flags */
	int c_flag = 0;
	int d_flag = 0;
	int h_flag = 0;
	int dbg_flag = 0;

	char* c_value = NULL;
	char* d_value = NULL;

	/* Possible short arguments */
	const char *shortOptions = "c:d:bh";

	struct option longOptions[] = {
		{"csv", 	required_argument, 	&c_flag, 	'c'},
		{"dwm", 	required_argument, 	&d_flag, 	'd'},
		{"help", 	no_argument, 		&h_flag, 	'h'},
		{"debug",	no_argument,		&dbg_flag,	'b'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, shortOptions, longOptions, NULL)) != -1) {
		switch (opt) {
			case 'c':
				c_flag = 1;
				c_value = optarg;
				break;
			case 'd': 
				d_flag = 1;
				d_value = optarg;
				break;
			case 'h':
				h_flag = 1;
				usage(name);
				break;
			case 'b':
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
		err(name, "Path to .csv package list is required\n");
		exit(EXIT_FAILURE);
	}

	if (!d_flag) {
		err(name, "URL to dwm repo is required\n");
		exit(EXIT_FAILURE);
	}

	for (int i = optind; i < argc; ++i) {
		/* Skip over any non-option arguments */
	}

	return EXIT_SUCCESS;
}

