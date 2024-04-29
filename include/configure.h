#ifndef CONFIGURE_H
#define CONFIGURE_H

/* Used to store various configurations and options
 * based on the arguments passed to the program. This
 * is later used by the main function to perform setup
 * actions accordingly. */
typedef struct Config {
	const char* aur_helper;
	const char* packages_csv_path;
	const char* dwm_git_url;
	const char* dwmblocks_git_url;
} Config;

int init_config(Config* config);
int opt_handler(int argc, char **argv);

#endif
