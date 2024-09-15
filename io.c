/*
 * Program to remove comments and minimize shader file.
 * Date: 2024-09-14
 * Author: Rubisetcie, Max Base
 * Repository: https://github.com/RubisetCie/glsl-minifier
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"

/* Read whole file into memory */
char* file_read(char *filename)
{
	FILE* file;
	char* buffer = NULL;
	long size;
	if ((file = fopen(filename, "rb")) == NULL)
	{
		fprintf(stderr, "Error: File not found: %s\n", filename);
		return NULL;
	}

	/* Retrieve the length of the file */
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	/* Allocate the buffer */
	if ((buffer = malloc(sizeof(char) * (size+1))) == NULL)
	{
		fputs("Error: Memory allocation failed!\n", stderr);
		goto END;
	}

	/* Don't forget the null byte */
	fread(buffer, 1, size, file);
	buffer[size] = '\0';

	END:

	fclose(file);
	return buffer;
}

/* Write a whole buffer to a file */
void file_write(char *filename, const char *buffer)
{
	FILE* file;
	if ((file = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "Error: Can't create the output file: %s\n", filename);
		return;
	}

	fwrite(buffer, strlen(buffer), 1, file);
	fclose(file);
}
