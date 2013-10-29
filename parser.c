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
        const char *t = getToken(&ts->codeStream);
        ts->length++;
        ts->tokens = (const char **)realloc(ts->tokens, ts->length * sizeof(const char *));
        ts->tokens[ts->length - 1] = t;
    }
}

const char *lookToken(TokenStream *ts, int shift){
    int i = ts->position + shift;
    return i < ts->length ? ts->tokens[i] : "";
}

const char *nextToken(TokenStream *ts){
    const char *res = lookToken(ts, 0);
    ts->position++;
    return res;
}

void skipTokens(TokenStream *ts, int n){
    ts->position += n;
}

bool isEndOfStream(TokenStream *ts){
    return ts->position >= ts->length;
}

Node parse(TokenStream *ts, int shift){
    LOG("parse", "begin");
    Node node = {"", 0, NULL};
    if(isEndOfStream(ts))
        return node;
    const char *token = nextToken(ts);
    while(!(strcmp(token, "\n") && strcmp(token, "\t")))
        token = nextToken(ts);
    node.name = token;
    while(!isEndOfStream(ts)){
        token = nextToken(ts);
        if(!strcmp(token, ")")) break;
        if(!strcmp(token, "\t")) continue;
        bool readGroup = false;
        if(!strcmp(token, "\n")){
            int shift_count = 0;
            bool end_big_cycle = false;
            while(!end_big_cycle){
                shift_count = 0;
                while(!strcmp(lookToken(ts, shift_count), "\t"))
                    ++shift_count;
                if(!strcmp(lookToken(ts, shift_count), "\n")){
                  skipTokens(ts, shift_count);
                  token = nextToken(ts);
                }
                else{
                    end_big_cycle = true;
                }
            }
            readGroup = shift_count == shift + 1;
            if(!readGroup){
                ts->position--;
                break;
            }
        }
        node.childs_length++;
        node.childs = (Node *) realloc(node.childs, node.childs_length * sizeof(Node));
        if(readGroup || !strcmp(token, "(")){
            node.childs[node.childs_length - 1] = parse(ts, (readGroup ? shift + 1 : shift));
        }
        else{
            node.childs[node.childs_length - 1].name = token;
            node.childs[node.childs_length - 1].childs_length = 0;
        }
    }
    return node;
}

void printTree(Node node, int shift){
    for(int i = 0; i < shift; i++)
        printf("  ");
    printf("%s\n", node.name);
    for(int i = 0; i < node.childs_length; i++)
        printTree(node.childs[i], shift + 1);
}
