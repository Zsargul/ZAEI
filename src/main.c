#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int opt;
	
	/* Argument flags */
	int csvFlag = 0;

	struct option longOptions[] = {
		{"csv", no_argument, &csvFlag, 1},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "c", long_options, NULL)) != -1) {
		switch (opt) {
			case 'c': /* -c */
				break;
			case 0: /* --csv */
				if (csvFlag) {
					/* Handle --csv flag */
				}
				break;
			case '?': /* Invalid option or missing argument */
				help();
				exit(EXIT_FAILURE);
		}
	}

	if (!csvFlag) {
		fprintf(stderr, "path to .csv package list is required\n");
		exit(EXIT_FAILURE);
	}

	for (int i = optind; i < argc; ++i) {
		/* Skip over any non-option arguments */
	}

	return 0;
}
