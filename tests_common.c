#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"

testIsDigit(){
   assert(isDigit("0"));
   assert(isDigit("-100"));
   assert(isDigit("1"));
   assert(!isDigit("1f"));
   assert(!isDigit("-1f"));
   assert(!isDigit(""));
   assert(!isDigit(" "));
}

int main(int argc, char **argv){
    testIsDigit();
}
