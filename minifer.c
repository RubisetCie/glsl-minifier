/*
 * Program to remove comments and minimize shader file.
 * Date: 2024-09-14
 * Author: Rubisetcie, Max Base
 * Repository: https://github.com/RubisetCie/glsl-minifier
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "minifer.h"

/* Define the macro conditions */
#define IS_NEWLINE(b)                  (b[i] == '\n' || b[i] == '\r')
#define IS_SINGLE_LINE_COMMENT(b)      (b[i] == '/' && b[i+1] == '/')
#define IS_MULTI_LINE_COMMENT_BEGIN(b) (b[i] == '/' && b[i+1] == '*')
#define IS_MULTI_LINE_COMMENT_END(b)   (b[i] == '*' && b[i+1] == '/')
#define IS_USELESS_CHAR(b)             (b[i] == ' ' || b[i] == '\t' || b[i] == '\n' || b[i] == '\r')
#define IS_IDENTIFIER_CHAR(b)          (isalpha(b[i]) || isdigit(b[i]) || b[i] == '_')

/* Define the limits */
#define IDENTIFIER_MAX 52

/* Remove all C++ and C-style comments */
char* remove_comments(char *buffer)
{
	char* new_buffer;
	register int i = 0, j = 0;

	/* Allocate a second buffer */
	if ((new_buffer = malloc(sizeof(char) * (strlen(buffer) + 1))) == NULL)
	{
		fputs("Error: Memory allocation failed!\n", stderr);
		return NULL;
	}

	/* Search for comments */
	while (buffer[i] != '\0')
	{
		/* Skip single line comment */
		if (IS_SINGLE_LINE_COMMENT(buffer))
		{
			while (!IS_NEWLINE(buffer))
			{
				if (buffer[i] == '\0')
					goto END;
				i++;
			}
		}
		/* Skip multi-line comment */
		else if (IS_MULTI_LINE_COMMENT_BEGIN(buffer))
		{
			i += 2;
			while (!IS_MULTI_LINE_COMMENT_END(buffer))
				i++;
			i += 2;
		}
		else
		{
			/* Copy characters to the new buffer */
			new_buffer[j++] = buffer[i++];
		}
	}

	END:

	/* Free the previous buffer */
	free(buffer);

	/* Don't forget the null byte */
	new_buffer[j] = '\0';

	return new_buffer;
}

/* Minify code in the buffer */
char* minify_code(char *code)
{
	char* new_code, last_preprocessor = 1;
	register int i = 0, j = 0;
	int word_start;

	/* Allocate a second buffer */
	if ((new_code = malloc(sizeof(char) * (strlen(code) + 1))) == NULL)
	{
		fputs("Error: Memory allocation failed!\n", stderr);
		return NULL;
	}

	/* Parse the code */
	while (code[i] != '\0')
	{
		/* Handle identifiers */
		if (IS_IDENTIFIER_CHAR(code))
		{
			/* Append the identifier to new_code */
			while (IS_IDENTIFIER_CHAR(code))
				new_code[j++] = code[i++];

			/* Skip whitespaces after the identifier */
			while (IS_USELESS_CHAR(code))
				i++;

			if (IS_IDENTIFIER_CHAR(code))
			{
				new_code[j++] = ' ';
				new_code[j++] = code[i++];
			}

			last_preprocessor = 0;
		}
		/* Handle preprocessor */
		else if (code[i] == '#')
		{
			char spaced = 0;
			if (!last_preprocessor)
				new_code[j++] = '\n';
			while (!(IS_NEWLINE(code) || code[i] == '\0'))
			{
				if (IS_USELESS_CHAR(code))
				{
					if (spaced)
					{
						i++;
						continue;
					}
					else
						spaced = 1;
				}
				else
					spaced = 0;
				new_code[j++] = code[i++];
			}
			new_code[j++] = '\n';
			last_preprocessor = 1;
		}
		/* Handle brackets */
		else if (code[i] == '{' || code[i] == '}')
		{
			new_code[j++] = code[i++];
			last_preprocessor = 0;
		}
		/* Handle semicolons */
		else if (code[i] == ';')
		{
			new_code[j++] = code[i++];
			while (IS_USELESS_CHAR(code))
				i++;
			last_preprocessor = 0;
		}
		/* Handle parentheses */
		else if (code[i] == '(' || code[i] == ')')
		{
			new_code[j++] = code[i++];
			last_preprocessor = 0;
		}
		/* Handle commas */
		else if (code[i] == ',')
		{
			new_code[j++] = code[i++];
			while (IS_USELESS_CHAR(code))
				i++;
			last_preprocessor = 0;
		}
		/* Skip all useless characters */
		else if (IS_USELESS_CHAR(code))
			i++;
		else
		{
			/* Write single quote chars */
			if (code[i] == '\'')
			{
				new_code[j++] = code[i++];
				while (code[i] != '\'' && code[i-1] == '\\')
					new_code[j++] = code[i++];
				new_code[j++] = code[i++];
			}
			/* Write double quotes string */
			else if (code[i] == '"')
			{
				new_code[j++] = code[i++];
				while (code[i] != '"' && code[i-1] == '\\')
					new_code[j++] = code[i++];
				new_code[j++] = code[i++];
			}
			else
				new_code[j++] = code[i++];

			last_preprocessor = 0;
		}
	}

	END:

	/* Free the previous buffer */
	free(code);

	/* Add both new line and null byte */
	if (!last_preprocessor)
		new_code[j++] = '\n';

	new_code[j] = '\0';

	return new_code;
}
