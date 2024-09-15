/*
 * Program to remove comments and minimize shader file.
 * Date: 2024-09-14
 * Author: Rubisetcie, Max Base
 * Repository: https://github.com/RubisetCie/glsl-minifier
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minifer.h"
#include "io.h"

static void display_help(const char *prg)
{
	printf("Usage: %s (options) (input 1) [(...) (input n)]\n\nOptions:\n  -o : Output file or output directory depending on the input.\n", prg);
}

static char* trim_extension(char *filename)
{
	char *dot = strrchr(filename, '.');
	*dot = '\0';
	return dot+1;
}

/* Entry point */
int main(int argc, char **argv)
{
	char *output = NULL, *buffer, isDir = 0;
	register int i = 1;

	if (argc < 2)
	{
		display_help(argv[0]);
		return 0;
	}

	/* Handling options */
	if (argv[1][0] == '-')
	{
		switch (argv[1][1])
		{
			case '?':
			case 'h':
				display_help(argv[0]);
				return 0;
			case 'o':
				if (argc > 4)
				{
					/* If multiple input follows, treat the out as a directory */
					isDir = 1;
				}
				if (argc <= 3)
				{
					fputs("Error: Incomplete command!\n", stderr);
					display_help(argv[0]);
					return 1;
				}
				output = argv[2];
				i = 3;

				/* Check the validity of the path */
#ifdef _WIN32
				if (!isDir && output[strlen(output) - 1] == '\\')
#else
				if (!isDir && output[strlen(output) - 1] == '/')
#endif
				{
					fputs("Error: The output path must not be a directory!\n", stderr);
					return 1;
				}
				break;
			default:
				fprintf(stderr, "Error: Unrecognized option: '%c'\n", argv[1][1]);
				display_help(argv[0]);
				return 1;
		}
	}

	/* Processing each file */
	for (; i < argc; i++)
	{
		/* First, read the content of the file into a buffer */
		if ((buffer = file_read(argv[i])) == NULL)
			goto NEXT;

		/* Then, remove the comments */
		if ((buffer = remove_comments(buffer)) == NULL)
			goto NEXT;

		/* Finally, proceed to the minification */
		if ((buffer = minify_code(buffer)) == NULL)
			goto NEXT;

		/* When the output file is specified */
        if (output)
        {
            if (isDir)
            {
                char *path;
				if ((path = malloc(strlen(output) + strlen(argv[i]) + 2)) == NULL)
				{
					fputs("Error: Memory allocation failed!\n", stderr);
					goto NEXT;
				}
				sprintf(path, "%s/%s", output, argv[i]);
				file_write(path, buffer);
                free(path);
            }
            else
				file_write(output, buffer);
        }
        /* When the output file is not specified */
        else
        {
            char *path;
            char ext[9];  /* Limit the length of extensions */
			if ((path = malloc(strlen(argv[i]) + 9)) == NULL)
			{
				fputs("Error: Memory allocation failed!\n", stderr);
				goto NEXT;
			}
            strcpy(path, argv[i]);
			strncpy(ext, trim_extension(path), 9);
			path = strcat(path, "_min.");
			path = strcat(path, ext);
			file_write(path, buffer);
            free(path);
        }

		NEXT:

		free(buffer);
	}

	return 0;
}
