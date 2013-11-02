#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "common.h"
#include "lexer.h"
#include "parser.h"

void fillTokenStream(TokenStream *ts, const char *source){
    ts->codeStream.source = source;
    ts->codeStream.position = 0;
    ts->position = 0;
    ts->length = 0;
    ts->tokens = 0;    
    while(!isEndOfCode(&ts->codeStream)){
        LexerToken t = getToken(&ts->codeStream);
        ts->length++;
        ts->tokens = (const char **)realloc(ts->tokens, ts->length * sizeof(LexerToken));
        ts->tokens[ts->length - 1] = t;
    }

    //MAKE IT BETTER
    unsigned int line = 1;
    unsigned int pos = 0;
    unsigned int pos_in_line = 0;
    for (int i = 0; i < ts->length; i++){
        while(pos != ts->tokens[i].pos){
            if (ts->codeStream.source[pos] == '\n'){
                line++;
                pos_in_line = 0;
            }
            pos_in_line++;
            pos++;
        }
        ts->tokens[i].line = line;
        //printf("%d %d %d %s\n", i, ts->tokens[i].line, ts->tokens[i].pos, ts->tokens[i].token);
        ts->tokens[i].pos = pos_in_line - strlen(ts->tokens[i].token);
    }
}

LexerToken lookToken(TokenStream *ts, int shift){
    int i = ts->position + shift;
    LexerToken tmp;
    tmp.token = "";
    return i < ts->length ? ts->tokens[i] : tmp;
}

LexerToken nextToken(TokenStream *ts){
    LexerToken res = lookToken(ts, 0);
    ts->position++;
    return res;
}

void skipTokens(TokenStream *ts, int n){
    ts->position += n;
}

bool isEndOfStream(TokenStream *ts){
    LOG("isEndOfStream", "called");
    return ts->position >= ts->length;
}

Node parse(TokenStream *ts, int shift){
    LOG("parse", "begin");
    Node node = {"", 0, 0, 0, NULL};
    if(isEndOfStream(ts))
        return node;
    LexerToken token = nextToken(ts);
    while(!(strcmp(token.token, "\n") && strcmp(token.token, "\t")))
        token = nextToken(ts);
    node.name = token.token;
    node.line = token.line;
    node.pos_in_line = token.pos;
    while(!isEndOfStream(ts)){
        token = nextToken(ts);
        if(!strcmp(token.token, ")"))
            break;
        if(!strcmp(token.token, "\t"))
            continue;
        bool readGroup = false;
        if(!strcmp(token.token, "\n")){
            int shift_count = 0;
            bool found = false;
            while(!found){
                shift_count = 0;
                while(!strcmp(lookToken(ts, shift_count).token, "\t"))
                    ++shift_count;
                if(!strcmp(lookToken(ts, shift_count).token, "\n"))
                    skipTokens(ts, shift_count + 1);
                else
                    found = true;
            }
            readGroup = shift_count == shift + 1;
            if(!readGroup){
                ts->position--;
                break;
            }
        }
        node.childs_length++;
        node.childs = (Node *) realloc(node.childs, node.childs_length * sizeof(Node));
        int i = node.childs_length - 1;
        if(readGroup || !strcmp(token.token, "(")){
            node.childs[i] = parse(ts, (readGroup ? shift + 1 : shift));
        }
        else{
            node.childs[i].name = token.token;
            node.childs[i].childs_length = 0;
            node.childs[i].childs = NULL;
            node.childs[i].line = token.line;
            node.childs[i].pos_in_line = token.pos;
        }
    }
    //printf("> %d %d %s\n", node.line, node.pos_in_line, node.name);
    return node;
}

void printTree(Node node, int shift){
    for(int i = 0; i < shift; i++)
        printf("  ");
    printf("%s\n", node.name);
    for(int i = 0; i < node.childs_length; i++)
        printTree(node.childs[i], shift + 1);
}
