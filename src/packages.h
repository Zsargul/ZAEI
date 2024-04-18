#ifndef PACKAGES_H
#define PACKAGES_H

#include "constants.h"

#define MAX_PKG_LEN 256

typedef struct Package {
	const char[MAX_PKG_LEN];
	int req;
	int on_aur;
} Package;

#endif
