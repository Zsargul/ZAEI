#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parsecsv.h"
#include "constants.h"

int csv_lines(const char* filename) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("Error opening file");
		return -1;
	}

	/* Skip header */
	char header[MAX_STR_LEN];
	if (fgets(header, sizeof(header), fp) == NULL) {
		fclose(fp);
		return -1; /* Error reading header */
	}

	int lineCount = 0;
	char buff[MAX_STR_LEN];
	while (fgets(buff, sizeof(buff), fp) != NULL) {
		/* Check if line ends with newline */
		if (buff[strlen(buff) - 1] == '\n') {
			lineCount++;
		}
	}

	fclose(fp);
	return lineCount;
}

int parse_package_list(const char* filename, Package *pkgs_array, int size) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("Error opening file");
		return -1;
	}

	/* Skip header */
	char header[MAX_STR_LEN];
	if (fgets(header, sizeof(header), fp) == NULL) {
		fclose(fp);
		return -1; /* Error reading header */
	}

	char buff[MAX_STR_LEN];
	while (fgets(buff, sizeof(buff), fp) != NULL) {
		
	}


	return 0;
}	

const char* get_field(char* line, int num) {
	const char* tok;

	tok = strtok(line, ",");
	while (tok != NULL) {
		if (!--num)
			return tok;

		tok = strtok(NULL, line;
	}
	return NULL;
}
