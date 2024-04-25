#ifndef CONFIGURE_H
#define CONFIGURE_H

/* Used to store various configurations and options
 * based on the arguments passed to the program. This
 * is later used by the main function to perform setup
 * actions accordingly. */
struct Config {
	int placeHolderOpt;
};

int init_config(int argc, int **argv);

#endif
