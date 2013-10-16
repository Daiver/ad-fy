#ifndef __COMMON_H__
#define __COMMON_H__

#define bool char
#define true 1
#define false !true

#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<malloc.h>
#include<string.h>

char *readFileAsLine2(char *input_file_name){
    FILE *fp;
    struct stat buf;
    fp=fopen(input_file_name,"r");
    fseek(fp, 0, SEEK_END);
    int pos = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *str = malloc(buf.st_size);
    while(!feof(fp))
    {
        char *s=malloc(1024);
        fgets(s,1024,fp);
        s[strlen(s)-1]='\0';
        printf("%s\n", s);
        strcat(str,s);
    }
    printf(">>>>\n");
    str[strlen(str) - 1] = '\0';
    return str;
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

#endif
