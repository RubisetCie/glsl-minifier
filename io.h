/*
 * Program to remove comments and minimize shader file.
 * Date: 2024-09-14
 * Author: Rubisetcie, Max Base
 * Repository: https://github.com/RubisetCie/glsl-minifier
 */

#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

char* file_read(char *filename);
void file_write(char *filename, const char *buffer);

#endif
