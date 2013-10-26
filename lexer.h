#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

struct TCodeStream{
    const char *source;
    unsigned int position;
};
typedef struct TCodeStream CodeStream;
extern const char *stopchars[];// = {"\0", "\t", "\n", "(", ")"};
extern const int stopchars_length;// = 5;
const char *getToken(CodeStream *stream);
bool isEndOfCode(CodeStream *ts);

#endif
