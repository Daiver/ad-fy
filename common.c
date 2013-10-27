#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <malloc.h>
#include <string.h>
#include "common.h"

bool isDigit(const char *s){
    if(strlen(s) == 0)
        return false;
    const char *str = s;
    if(*str == '-'){
        str++;
        if(*str == '\0')
            return false;
    }
    while (*str != '\0'){
        if (*str < 48 || *str > 57) 
            return false;
        str++;
    }
    return true;
}

char *readFileAsLine(char *input_file_name){
    char *file_contents;
    long input_file_size;
    FILE *input_file = fopen(input_file_name, "rb");
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = malloc((input_file_size) * (sizeof(char)));
    fread(file_contents, sizeof(char), input_file_size, input_file);
    fclose(input_file);
    //file_contents[input_file_size] = 0;
    return file_contents;
}
