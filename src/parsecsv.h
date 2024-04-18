#ifndef PARSECSV_H
#define PARSECSV_H

#include "packages.h"

int csv_lines(const char* filename);
int parse_package_list(const char* filename, Package *pkgs_array, int size);
const char* get_field(char* line, int num);

#endif
