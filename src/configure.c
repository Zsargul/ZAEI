#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "constants.h"
#include "util/miscutils.h"
#include "configure.h"

/* Returns values:
 * -1: Critical error
 *  0: No errors/warnings
 */
int init_config(Config* config) {
	int ret = 0;

	config_t cfg;
	config_init(&cfg);

	if (config_read_file(&cfg, CONFIG_FILE) != CONFIG_TRUE) {
		fprintf(stderr, "%s: Libconfig error: %s:%d - %s\n", CRIT, config_error_file(&cfg), 
				config_error_line(&cfg), config_error_text(&cfg));
		ret = -1;
		return ret;
	}

	/* TODO: Make some of these options (like dwm/dwmblocks) optional, i.e, let them be commented out
	 * and don't exit with failure if they are missing:
	 * NOTE: Will need to use extra intermediate variables to store values and check if they even exist. If a value
	 * is commented out for example, it should be set to NULL in the Config struct. It should later be checked if that
	 * value is NULL before actually doing any setup with it.
	 * */

	/* Get AUR helper */
	if (config_lookup_string(&cfg, "aur_helper", &config->aur_helper)) {
		dbg_fprintf(stdout, "%s: Config - Found aur_helper: %s\n", INFO, config->aur_helper);
	} else {
		fprintf(stderr, "%s: Unable to find 'aur_helper' value in %s!\n", CRIT, CONFIG_FILE);
		ret = -1;
		return ret;
	}
	
	/* TODO: Clean up the output messages below */

	/* Get path to packages .csv file */
	if (config_lookup_string(&cfg, "packages_csv_path", &config->packages_csv_path)) { 
		dbg_fprintf(stdout, "%s: Config - Found packages_csv_path: %s\n", INFO, config->packages_csv_path);
	} else {
		fprintf(stderr, "%s: Unable to find 'packages_file' field in config file!\n", CRIT);
		ret = -1;
		return ret;
	}

	/* [OPTIONAL]  Get DWM git URL */
	if (config_lookup_string(&cfg, "dwm_git_url", &config->dwm_git_url)) {
		dbg_fprintf(stdout, "%s: Config - Found dwm_git_url: %s\n", INFO, config->dwm_git_url);
	} else {
		fprintf(stderr, "%s: Didn't find field 'dwm_git_url' in config file. Skipping...\n", INFO);
		config->dwm_git_url = NULL;
	}

	/* [OPTIONAL] Get DWM blocks git URL */
	if (config_lookup_string(&cfg, "dwmblocks_git_url", &config->dwmblocks_git_url)) {
		dbg_fprintf(stdout, "%s: Config - Found dwmblocks_git_url: %s\n", INFO, config->dwmblocks_git_url);
	} else {
		fprintf(stdout, "%s: Didn't find field 'dwmblocks_git_url' in config file. Skipping...\n", INFO);
		config->dwmblocks_git_url = NULL;
	}

	/* [OPTIONAL] Get DWM install directory */
	if (config_lookup_string(&cfg, "dwm_dir", &config->dwm_dir)) {
		dbg_fprintf(stdout, "%s: Config - Found dwm_dir: %s\n", INFO, config->dwm_dir);
	} else {
		fprintf(stdout, "%s: Didn't find 'dwm_dir' value in config file. Skipping...\n", INFO);
	}

	/* [OPTIONAL] Get DWM blocks install directory */
	if (config_lookup_string(&cfg, "dwmblocks_dir", &config->dwmblocks_dir)) {
		dbg_fprintf(stdout, "%s: Config - Found dwmblocks_dir: %s\n", INFO, config->dwmblocks_dir);
	} else {
		fprintf(stdout, "%s: Didn't find 'dwmblocks_dir' value in config file. Skipping...\n", CRIT);
	}

	/* Deallocate memory for cfg object before returning */
	config_destroy(&cfg);

	return ret;	
}

/* Parse any command line options. */
void opt_handler(int argc, char **argv) {
	char* name = argv[0];
	int opt;

	/* Flags */
	int d_flag = 0;
	int h_flag = 0;

	/* TODO: Debug and check that ALL of these work, short and long options */
	/* Options */
	struct option longOptions[] = {
		{"help", 	no_argument, 		&h_flag, 	'h'},
		{"debug",	no_argument,		&d_flag,	'd'},
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
			case 'd':
				d_flag = 1;
				enable_debug_mode();
				dbg_fprintf(stdout, "! Enabled debug messages !\n");
				break;
			case '?': /* Invalid option or missing argument */
				err_usage(name, "Unrecognised argument\n");
				exit(EXIT_FAILURE);
		}
	}

	for (int i = optind; i < argc; ++i) {
		/* Skip over any non-option arguments */
	}
}
