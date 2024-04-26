#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "util/miscutils.h"

int main(int argc, char **argv) {
	config_t cfg;
	init_config(&cfg);	

	return EXIT_SUCCESS;
}

