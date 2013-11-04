#include "eapi.h"
#include "hi.h"

int square(int j){
    hu();
    return j * j;
}

int dbl(int j){
    return j * 2;
}


void getExtContent(const char ***names, int *len){
    const int l = 4;
    *len = l;
    *names = malloc(sizeof(char*) * l);
    (*names)[0] = "dbl";
    (*names)[1] = "dbl";
    (*names)[2] = "sq";
    (*names)[3] = "square";
}
