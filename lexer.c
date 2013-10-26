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

const char *getToken(CodeStream *stream){
    int spaceCount = 0;
    while(stream->source[stream->position] == ' '){
        ++spaceCount;
        ++stream->position;
        if(spaceCount == 4)
          return "\t";   
    }
    char *ch = NULL;
    if(isStopChar(stream->source[stream->position], &ch)){
        stream->position += (ch != '\0');
        return ch;
    }
    int i = stream->position;
    while(!isStopChar(stream->source[i], &ch))
        ++i;
    int len = i - stream->position;
    char *token = (char *) malloc((len + 1) * sizeof(char));
    strncpy(token, stream->source + stream->position, len);    
    token[len] = '\0';
    stream->position = i;
    return token;
}

bool isEndOfCode(CodeStream *ts){
    return ts->source[ts->position] == '\0';
}
