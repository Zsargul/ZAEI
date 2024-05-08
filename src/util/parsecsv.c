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
	printf("1");
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

		for (size_t i = 0; i < (sizeof(fields) / sizeof(fields[0])); i++)
			fields[i] = get_field(header, i);
	}

	int lineNo = 0;
	char buff[MAX_STR_LEN];
	while ( (fgets(buff, sizeof(buff), fp) != NULL) && (lineNo != size) ) {
		const char* name;
		int req;
		int onAur;
		
		char* tmp = strdup(buff);
		
		/* Get 0th (first) field */
		name = get_field(tmp, 0);
		if (name == NULL) {
			fprintf(stderr, "Error parsing csv. Value in field '%s' is null.", fields[0]);
			return 1;
		}

		const char* reqField = get_field(tmp, 1);
		const char* onAurfield = get_field(tmp, 2);

		/* Get 1st field */
		if (strcmp(reqField, "yes") == 0) {
			req = 1;
		} else if (strcmp(reqField, "no") == 0) {
			req = 0;
		} else {
			fprintf(stderr, "Unknown value encountered in csv field %d (%s)", 1, fields[1]);
			return 1;
		}

		/* Get 2nd field */
		if (strcmp(onAurfield, "yes") == 0) {
			onAur = 1;
		} else if (strcmp(onAurfield, "no") == 0) {
			onAur = 0;
		} else {
			fprintf(stderr, "Unknown value encountered in csv field %d (%s)", 2, fields[2]);
			return 1;
		}
		
		/* Create new package as struct and add to array */
		Package pkg = { {name}, req,  onAur };
		pkgs_array[lineNo] = pkg;		

		free(tmp);
	}
	return 0;
}	

const char* get_field(char* line, int num) {
	const char* tok;

	tok = strtok(line, ",");
	while (tok != NULL) {
		if (!num--)
			return tok;

		tok = strtok(NULL, line);
	}
	return NULL;
}
