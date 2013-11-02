#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "lexer.h"

const char *stopchars[] = {" ", "\0", "\t", "\n", "(", ")"};
const int stopchars_length = sizeof(stopchars)/sizeof(char *);

bool isStopChar(char c, char **ch){
  for(int i = 0; i < stopchars_length; ++i){
    if(*stopchars[i] == c){
	*ch = stopchars[i];
        return true;
    }
  }
  return false;
}

LexerToken getToken(CodeStream *stream){
    LexerToken res;
    int spaceCount = 0;
    while(stream->source[stream->position] == ' '){
        ++spaceCount;
        ++stream->position;
        if(spaceCount == 4){
            res.token = "\t";
            res.pos = stream->position;
            return res;
        }
    }
    char *ch = NULL;
    if(isStopChar(stream->source[stream->position], &ch)){
        stream->position += (ch != '\0');
        res.token = ch;
        res.pos = stream->position;
        return res;
    }
    int i = stream->position;
    while(!isStopChar(stream->source[i], &ch))
        ++i;
    int len = i - stream->position;
    char *token = (char *) malloc((len + 1) * sizeof(char));
    strncpy(token, stream->source + stream->position, len);    
    token[len] = '\0';
    stream->position = i;
    res.token = token;
    res.pos = stream->position;
    return res;
}

bool isEndOfCode(CodeStream *ts){
    return ts->source[ts->position] == '\0';
}
