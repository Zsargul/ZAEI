#ifndef PACKAGES_H
#define PACKAGES_H

#include "constants.h"

#define MAX_PKG_LEN 256

typedef struct Package {
	const char* name[MAX_PKG_LEN];
	int req;
	int onAur;
} Package;

int install_packages();
int install_package(Package *pkg);

#endif
