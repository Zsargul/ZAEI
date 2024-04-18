#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "parsecsv.h"
#include "constants.h"

int csv_lines(const char* filename) {
	struct stat statBuff;
	if (stat (filename, &statBuff) != 0) {
		perror("File \"%s\" does not exist");
		return -1;
	}

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
