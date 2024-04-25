#ifndef CONFIGURE_H
#define CONFIGURE_H

/* Used to store various configurations and options
 * based on the arguments passed to the program. This
 * is later used by the main function to perform setup
 * actions accordingly. */
struct Config {
	const char* package_csv_path;
	const char* dwm_git_url;

};

int init_config(int argc, int **argv);

#endif
