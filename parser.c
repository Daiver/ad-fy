#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "common.h"
#include "lexer.h"
#include "parser.h"

void fillTokenStream(TokensStream *ts, const char *source){
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

const char *lookToken(TokensStream *ts, int shift){

//Are you shure it is a good idea to perform such corretions? 
//If (shift > ts->length) it is more likely that shift is incorrect

//    if(ts->position + shift >= ts->length){
//        int diff_length = ts->position + shift - ts->length + 1;
//        for(int i = 0; i < diff_length && !isEndOfCode(&ts->codeStream); i++){
//            const char *t = getToken(&ts->codeStream);
//            ts->length++;
//            ts->tokens = (const char **)realloc(ts->tokens, ts->length * sizeof(const char *));
//            ts->tokens[ts->length - 1] = t;
//        }
//    }


    if(ts->position + shift >= ts->length) return "";
    return ts->tokens[ts->position + shift];
}

const char *nextToken(TokensStream *ts){
    const char *res = lookToken(ts, 0);
    ts->position++;
    return res;
}

bool isEndOfStream(TokensStream *ts){
    return ts->position >= ts->length;
}

Node parse(TokensStream *ts, int shift){
    LOG("parse", "begin");
    Node res = {"", 0, 0};
    LOG("parse", "checking end of stream");
    if(isEndOfStream(ts)) 
        return res;
    const char *token = nextToken(ts);
    while(strcmp(token, "\n") == 0)
        token = nextToken(ts);
    while(strcmp(token, "\t") == 0)
        token = nextToken(ts);

    LOG("parse", "token");
    res.name = token;
    LOG("parse", token);
    if(!token) return res;
    while(!isEndOfStream(ts)){
        LOG("parse", "child token");
        token = nextToken(ts);
        LOG("parse", token);
        if(!token) return res;
        LOG("parse", "checking if )");
        if(strcmp(token, ")") == 0) break;
        LOG("parse", "checking if tab");
        if(strcmp(token, "\t") == 0) continue;
        bool readGroup = false;
        LOG("parse", "checking if end of line");
        if(strcmp(token, "\n") == 0){
            int shift_count = 0;
            bool end_big_cycle = false;
            while(!end_big_cycle){
                shift_count = 0;
                while(strcmp(lookToken(ts, shift_count), "\t") == 0){
                    ++shift_count;
                }
                if(strcmp(lookToken(ts, shift_count), "\n") == 0){
                    token = lookToken(ts, shift_count);
                    ts->position += shift_count + 1;
                }
                else
                    end_big_cycle = true;
            }
            readGroup = shift_count == shift + 1;
            if(!readGroup){
                ts->position--;
                break;
            }
        }
        LOG("parse", "childs memory reallocation");
        res.childs_length++;
        res.childs = (Node *) realloc(res.childs, res.childs_length * sizeof(Node));
        LOG("parse", "checking if ( or readGroup");
        if(strcmp(token, "(") == 0 || readGroup){
            LOG("parse", "true");
            res.childs[res.childs_length - 1] = parse(ts, (readGroup ? shift + 1 : shift));
        }
        else{
            LOG("parse", "false");
            res.childs[res.childs_length - 1].name = token;
            res.childs[res.childs_length - 1].childs_length = 0;
        }
    }

    LOG("parse", "native end");
    return res;
}

void printTree(Node node, int shift){
    for(int i = 0; i < shift; i++)
        printf("  ");
    printf("%s\n", node.name);
    for(int i = 0; i < node.childs_length; i++)
        printTree(node.childs[i], shift + 1);
}
