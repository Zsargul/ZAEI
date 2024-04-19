#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char **argv) {
	int opt;
	
	/* Argument flags */
	int c_flag = 0;
	int d_flag = 0;
	int h_flag = 0;
	int debug_flag = 0;

	char* c_value = NULL;
	char* d_value = NULL;

	/* Possible short arguments */
	const char *optstring = "c:d:";

	struct option longOptions[] = {
		{"csv", 	required_argument, 	&c_flag, 	'c'},
		{"dwm", 	required_argument, 	$d_flag, 	'd'},
		{"help", 	no_argument, 		&h_flag, 	'h'},
		{"debug",	no_argument,		$dbg_flag,	0},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "c", longOptions, NULL)) != -1) {
		switch (opt) {
			case 'c': /* -c */
				c_flag = 1;
				break;
			case 'd': /* -d */
				d_flag = 1;
				d_value = optarg;
				break;
			case '?': /* Invalid option or missing argument */
				help();
				exit(EXIT_FAILURE);
		}
	}

	/* Check for required arguments */
	if (!c_flag) {
		fprintf(stderr, "Path to .csv package list is required\n");
		exit(EXIT_FAILURE);
	}

	if (!d_flag) {
		fprintf(stderr, "URL to dwm repo is required\n");
		exit(EXIT_FAILURE);
	}

	for (int i = optind; i < argc; ++i) {
		/* Skip over any non-option arguments */
	}

	return EXIT_SUCCESS;
}
