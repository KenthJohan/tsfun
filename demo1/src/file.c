#include "file.h"

#include <stdlib.h>
#include <stdio.h>

char *file_read_allocated(const char *path)
{
	FILE *file = fopen(path, "r");
	if (!file) {
		perror("Unable to open file");
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	rewind(file);

	char *content = malloc(size + 1);
	if (!content) {
		perror("Unable to allocate memory");
		exit(EXIT_FAILURE);
	}
	fread(content, 1, size, file);
	fclose(file);

	content[size] = '\0';
	return content;
}