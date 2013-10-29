#include "eapi.h"

void getExtContent(const char ***names, int *len){
    const int l = 2;
    *len = l;
    *names = malloc(sizeof(char*) * l);
    (*names)[0] = "h";
    (*names)[1] = "hello";
}
