/*
 * Program to remove comments and minimize shader file.
 * Date: 2024-09-14
 * Author: Rubisetcie, Max Base
 * Repository: https://github.com/RubisetCie/glsl-minifier
 */

#ifndef MINIFIER_H_INCLUDED
#define MINIFIER_H_INCLUDED

char* remove_comments(char* buffer);
char* minify_code(char* code);

#endif
