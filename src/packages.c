#include <stdlib.h>

#include "packages.h"
#include "parsecsv.h"

int install_packages() {
	int no_of_packages = csv_lines(PACKAGES_LIST_FILE);
	if (no_of_packages == -1)
		exit(1);

	Package pkgs[no_of_packages]; /* Array of packages to install */

	if (no_of_packages != (sizeof(pkgs) / sizeof(pkgs[0]))) {
		fprintf(stderr, "Error creating pkgs array. Exiting.\n");
		exit(1);
	}

	if (parse_package_list(PACKAGES_LIST_FILE, pkgs, no_of_packages) == -1)
		exit(1);

	for (size_t i = 0; i < no_of_packages; i++) {
		int ret = install_package(pkgs[i]);
		
		if (ret == -1) {
			fprint(stderr, "%d: Package \"%s\", was not installed successfully, but it is not required.\n", i, pkgs[i].name);
		} else if (ret == -2) {
			fprintf(stderr, "%d: REQUIRED Package \"%s\", was not installed successfully. Exiting.\n", i, pkgs[i].name);
			exit(1);
		} else {
			fprintf(stdout, "%d: %s installed successfully\n", pkgs[i].name);
		}
	}


	return 0;
}

int install_package(Package *pkg) {
	int mustInstall = pkg->req;

	if (pkg->onAur) { /* Use AUR helper for installing this package */
	
	} else { /* Install from official repos */
	
	}

	return 0;
}

