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

struct TLexerToken{
    const char *token;
    int line;
    int pos;
};
typedef struct TLexerToken LexerToken;

extern const char *stopchars[];// = {"\0", "\t", "\n", "(", ")"};
extern const int stopchars_length;// = 5;
LexerToken getToken(CodeStream *stream);
//const char *getToken(CodeStream *stream);
bool isEndOfCode(CodeStream *ts);

#endif
