//#define NDEBUG 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lexer.h"
#include "common.h"

void testIsEndOfCode(){
   CodeStream cs = {"hell low", 0};
   while(cs.position < strlen(cs.source)){
       assert(!isEndOfCode(&cs));
       ++cs.position;
   }
   assert(isEndOfCode(&cs));
}

void testGetToken(){
  CodeStream cs = {"tokenA    tokenB (tokenC)", 0};
  assert(!strcmp(getToken(&cs), "tokenA"));
  assert(!strcmp(getToken(&cs), "\t"));
  assert(!strcmp(getToken(&cs), "tokenB"));
  assert(!strcmp(getToken(&cs), "("));
  assert(!strcmp(getToken(&cs), "tokenC"));
  assert(!strcmp(getToken(&cs), ")"));
  assert(!strcmp(getToken(&cs), ""));
}

int main(int arc, char **argv){
    testIsEndOfCode();
    testGetToken();
}
