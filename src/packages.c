#include <stdlib.h>
#include <stdio.h>

#include "packages.h"
#include "parsecsv.h"

int install_packages() {
	int no_of_packages = csv_lines(PACKAGES_LIST_FILE);
	if (no_of_packages == -1)
		exit(EXIT_FAILURE);

	Package pkgs[no_of_packages]; /* Array of packages to install */

	if (no_of_packages != (sizeof(pkgs) / sizeof(pkgs[0]))) {
		fprintf(stderr, "Error creating pkgs array. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	if (parse_package_list(PACKAGES_LIST_FILE, pkgs, no_of_packages) == -1)
		exit(EXIT_FAILURE);

	for (size_t i = 0; i < no_of_packages; i++) {
		Package* pkgPtr = &pkgs[i];
		int ret = install_package(pkgPtr);
		
		if (ret == -1) {
			fprintf(stderr, "%zd: Package \"%s\", was not installed successfully, but it is not required.\n", i, pkgs[i].name);
		} else if (ret == -2) {
			fprintf(stderr, "%zd: REQUIRED Package \"%s\", was not installed successfully. Exiting.\n", i, pkgs[i].name);
			exit(EXIT_FAILURE);
		} else {
			fprintf(stdout, "%zd: %s installed successfully\n", i,pkgs[i].name);
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

