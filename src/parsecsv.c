#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parsecsv.h"
#include "constants.h"

int csv_lines(const char* filename) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file %s", filename);
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
		fprintf(stderr, "Error opening file %s", filename);
		return -1;
	}

	/* Get header fields */
	const char* fields[CSV_FIELDS];
	char header[MAX_STR_LEN];
	if (fgets(header, sizeof(header), fp) == NULL) {
		fclose(fp);
		return -1; /* Error reading header */
	} else {
		if (CSV_FIELDS != (sizeof(fields) / sizeof(fields[0]))) {
			fprintf(stderr, "fields[] array is not the same size as the amount of fields in %s\n", PACKAGE_LIST_FILE);
			return -1;
		}

		for (size_t i = 0; i < (sizeof(fields) / sizeof(fields[0])); i++)
			fields[i] = get_field(header, i);
	}

	lineNo = 0;
	char buff[MAX_STR_LEN];
	while ( (fgets(buff, sizeof(buff), fp) != NULL) && (lineNo != size) ) {
		const char* name;
		int req;
		int onAUR;
		
		const char* tmp = strdup(buff);
		
		/* Get 0th (first) field */
		const char* name = get_field(buff, 0);
		if (name == NULL) {
			fprintf(stderr, "Error parsing csv. Field 'name' is null.");
			return -1
		}

		const char* reqField = get_field(buff, 1);
		const char* onAURfield = get_field(buff, 2);

		/* Get 1st field */
		if (strcmp(reqField, "yes") == 0) {
			req = 1;
		} else if (strcmp(reqField, "no") == 0) {
			req = 0;
		} else {
			fprintf(stderr, "Unknown value encountered in csv field %d (%s)", 1, fields[1]);
			return -1;
		}

		/* Get 2nd field */
		if (strcmp(onAURfield, "yes") == 0) {
			onAUR = 1;
		} else if (strcmp(onAURfield, "no") == 0) {
			onAUR = 0;
		} else {
			fprintf(stderr, "Unknown value encountered in csv field %d (%s)", 2, fields[2]);
			return -1;
		}
		
		/* Create new package as struct and add to array */
		Package pkg = { .name = name, .req = req, .onAUR = onAUR };
		pkgs_array[lineNo] = pkg;		
	}
	return 0;
}	

const char* get_field(char* line, int num) {
	const char* tok;

	tok = strtok(line, ",");
	while (tok != NULL) {
		if (!num--)
			return tok;

		tok = strtok(NULL, line;
	}
	return NULL;
}
