#include "eapi.h"

int anti(int j){
    return -j;
}

int add10(int j){
    return j + 10;
}

int zero(int i){
    return 0;
}

void getExtContent(const char ***names, int *len){
    const int l = 6;
    *len = l;
    *names = malloc(sizeof(char*) * l);
    (*names)[0] = "anti";
    (*names)[1] = "anti";
    (*names)[2] = "add10";
    (*names)[3] = "add10";
    (*names)[4] = "zero";
    (*names)[5] = "zero";
}
