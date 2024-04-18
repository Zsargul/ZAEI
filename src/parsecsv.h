#ifndef PARSECSV_H
#define PARSECSV_H

int csv_lines(const char* filename);
int parse_package_list(Package *pkgs_array, int size);
const char* get_field(char* line, int num);

#endif
