#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util/parsecsv.h"
#include "constants.h"
#include "packages.h"
#include "util/logging.h"

int csv_lines(const char* filename) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		log_msg(stderr, ERR, "csv_lines(): Error opening file %s.\n", filename);
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

/* Return values:
 * 1: Critical error
 * 0: Success
 */
int parse_package_list(const char* filename, Package *pkgs_array, int size) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file %s", filename);
		return 1;
	}

	/* Get header fields */
	const char* fields[CSV_FIELDS];
	char header[MAX_STR_LEN];
	if (fgets(header, sizeof(header), fp) == NULL) {
		fclose(fp);
		return 1; /* Error reading header */
	} else {
		if (CSV_FIELDS != (sizeof(fields) / sizeof(fields[0]))) {
			fprintf(stderr, "fields[] array is not the same size as the amount of fields in %s\n", filename);
			return 1;
		}

		for (int i = 0; i < (sizeof(fields) / sizeof(fields[0])); i++) {
			fields[i] = get_field(strdup(header), i+1);
		}
	}

	for (int i = 0; i < CSV_FIELDS; i++) {
		if (fields[i] == NULL) {
			log_msg(stderr, ERR, "Error parsing headers for csv file. Header at index fields[%d] is null.\n", i);
			return 1;
		}
	}

	int lineNo = 0;
	char buff[MAX_STR_LEN];
	while ( (fgets(buff, sizeof(buff), fp) != NULL) ) {
		const char* name;
	    const char*	reqField;
		const char* onAurField;
		int req;
		int onAur;
		
		name = get_field(strdup(buff), 1);
		if (name == NULL) {
			log_msg(stderr, ERR, "Error parsing csv. Value in field '%s' is null.", fields[0]);
			return 1;
		}

		reqField = get_field(strdup(buff), 2);
		if (reqField == NULL) {
			log_msg(stderr, ERR, "Error parsing csv. Value in field '%s' is null.", fields[1]);
			return 1;
		}

		onAurField = get_field(strdup(buff), 3);
		if (onAurField == NULL) {
			log_msg(stderr, ERR, "Error parsing csv. Value in field '%s' is null.", fields[2]);
			return 1;
		}

		/* Get 1st field */
		if (strcmp(reqField, "yes") == 0) {
			req = 1;
		} else if (strcmp(reqField, "no") == 0) {
			req = 0;
		} else {
			log_msg(stderr, ERR, "Unknown value encountered in csv field %d (%s)", 1, fields[1]);
			return 1;
		}

		/* Get 2nd field */
		if (strcmp(onAurField, "yes") == 0) {
			onAur = 1;
		} else if (strcmp(onAurField, "no") == 0) {
			onAur = 0;
		} else {
			log_msg(stderr, ERR, "Unknown value encountered in csv field %d (%s)", 2, fields[2]);
			return 1;
		}
		
		/* Create new package as struct and add to array */
		Package pkg = { name, req, onAur };
		pkgs_array[lineNo] = pkg;		

		lineNo++;
	}
	return 0;
}	

/* Uses strsep() instead of strtok() to allow for parsing empty fields in csv.
 * Index of fields on each line is 1 indexed, not 0. */
const char* get_field(char* line, int num) {
	char* tok;

	for (tok = strsep(&line, ",");
			tok;
			tok = strsep(&line, ",\n")) {
		if (!--num) {
			log_msg(stdout, INFO, "%s\n", tok);
			return tok;
		}
	}
	return NULL;
}
