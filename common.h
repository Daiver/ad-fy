#ifndef __COMMON_H__
#define __COMMON_H__

#define bool char
#define true 1
#define false !true

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <malloc.h>
#include <string.h>

int checkIsNumber(const char *s);
char *readFileAsLine(char *input_file_name);

#endif
