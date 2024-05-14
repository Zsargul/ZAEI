#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "constants.h"
#include "util/miscutils.h"
#include "configure.h"
#include "util/logging.h"

/* Returns values:
 * -1: Critical error
 *  0: No errors/warnings
 */
int init_config(Config* config) {
	config_t cfg;
	config_init(&cfg);

	if (config_read_file(&cfg, CONFIG_FILE) != CONFIG_TRUE) {
		log_msg(stderr, ERR, "Libconfig error: %s:%d - %s\n", config_error_file(&cfg),
				config_error_line(&cfg), config_error_text(&cfg));
		return -1;
	}

	/* TODO: Make some of these options (like dwm/dwmblocks) optional, i.e, let them be commented out
	 * and don't exit with failure if they are missing:
	 * NOTE: Will need to use extra intermediate variables to store values and check if they even exist. If a value
	 * is commented out for example, it should be set to NULL in the Config struct. It should later be checked if that
	 * value is NULL before actually doing any setup with it.
	 * */

	/* Get AUR helper */
	if (config_lookup_string(&cfg, "aur_helper", &config->aur_helper)) {
		log_msg(stdout, INFO, "CONFIG: aur_helper = %s\n", config->aur_helper);
	} else {
		log_msg(stderr, ERR, "CONFIG: Error getting value for field aur_helper in %s\n", CONFIG_FILE);
		return -1;
	}
	
	/* TODO: Clean up the output messages below */

	/* Get path to packages .csv file */
	if (config_lookup_string(&cfg, "packages_csv_path", &config->packages_csv_path)) { 
		log_msg(stdout, INFO, "CONFIG: packages_csv_path = %s\n", config->packages_csv_path);
	} else {
		log_msg(stderr, ERR, "CONFIG: Error getting value for field packages_file in %s\n", CONFIG_FILE);
		return -1;
	}

	/* [OPTIONAL]  Get DWM git URL */
	if (config_lookup_string(&cfg, "dwm_git_url", &config->dwm_git_url)) {
		log_msg(stdout, INFO, "CONFIG: dwm_git_url = config->dwm_git_url\n");
	} else {
		/* TODO: Polish this optional parameter handling logic up later */
		log_msg(stderr, ERR, "CONFIG: Error getting value for field dmw_git_url in %s\n", CONFIG_FILE);
		config->dwm_git_url = NULL;
	}

	/* [OPTIONAL] Get DWM blocks git URL */
	if (config_lookup_string(&cfg, "dwmblocks_git_url", &config->dwmblocks_git_url)) {
		log_msg(stdout, INFO, "CONFIG: dwmblocks_git_url = %s\n", config->dwmblocks_git_url);
	} else {
		log_msg(stdout, ERR, "CONFIG: Error getting value for field dwmblocks_git_url in %s\n", CONFIG_FILE);
		config->dwmblocks_git_url = NULL;
	}

	/* [OPTIONAL] Get DWM install directory */
	if (config_lookup_string(&cfg, "dwm_dir", &config->dwm_dir)) {
		log_msg(stdout, INFO, "CONFIG: dwm_dir = %s\n", config->dwm_dir);
	} else {
		log_msg(stderr, ERR, "CONFIG: Error getting value for field dwm_dir in %s\n", CONFIG_FILE);
	}

	/* [OPTIONAL] Get DWM blocks install directory */
	if (config_lookup_string(&cfg, "dwmblocks_dir", &config->dwmblocks_dir)) {
		log_msg(stdout, INFO, "CONFIG: dwmblocks_dir = %s\n", config->dwmblocks_dir);
	} else {
		log_msg(stdout, ERR, "CONFIG: Error getting value for field dwmblocks_dir in %s\n", CONFIG_FILE);
	}

	return 0;	
}

/* Parse any command line options. */
void opt_handler(int argc, char **argv) {
	char* name = argv[0];
	int opt;

	/* Flags */
	int h_flag = 0;

	/* TODO: Debug and check that ALL of these work, short and long options */
	/* Options */
	struct option longOptions[] = {
		{"help", 	no_argument, 		&h_flag, 	'h'},
		{0, 0, 0, 0}
	};

	const char *shortOptions = "dh";

	while ((opt = getopt_long(argc, argv, shortOptions, longOptions, NULL)) != -1) {
		switch (opt) {
			case 'h':
				h_flag = 1;
				usage(name);
				exit(EXIT_SUCCESS);
				break;
			case '?': /* Invalid option or missing argument */
				log_msg(stderr, ERR, "Unrecognised argument \"%s\".\n", opt);
				exit(EXIT_FAILURE);
		}
	}

	for (int i = optind; i < argc; ++i) {
		/* Skip over any non-option arguments */
	}
}
