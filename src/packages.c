#include <stdlib.h>

#include "packages.h"
#include "parsecsv.h"

int install_packages() {
	int no_of_packages = csv_lines(PACKAGES_LIST);
	if (no_of_packages == -1)
		exit(1);

	Package pkgs[no_of_packages];

	return 0;
}

int install_package(Package *pkg) {
	if (pkg->onAur) { /* Use AUR helper for installing this package */
	
	} else { /* Install from official repos */
	
	}

	return 0;
}

