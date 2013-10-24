#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "lexer.h"

struct TTokenStream{
    CodeStream codeStream;
    const char **tokens;
    unsigned int length;
    unsigned int position;  
};

struct TNode{
    const char *name;
    unsigned int childs_length;
    struct TNode *childs;
}; 

typedef struct TTokenStream TokensStream;
typedef struct TNode Node;

void fillTokenStream(TokensStream *ts, const char *source);
const char *lookToken(TokensStream *ts, int shift);
const char *nextToken(TokensStream *ts);
bool isEndOfStream(TokensStream *ts);
Node parse(TokensStream *ts, int shift);
void printTree(Node node, int shift);

#endif
