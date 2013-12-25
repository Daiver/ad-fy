#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "lexer.h"
#include "eapi.h"

struct TTokenStream{
    CodeStream codeStream;
    LexerToken *tokens;
    unsigned int length;
    unsigned int position;
};
typedef struct TTokenStream TokenStream;
void fillTokenStream(TokenStream *ts, const char *source);
LexerToken lookToken(TokenStream *ts, int shift);
LexerToken nextToken(TokenStream *ts);
bool isEndOfStream(TokenStream *ts);
Node parse(TokenStream *ts, int shift);
void printTree(Node node, int shift);

#endif
