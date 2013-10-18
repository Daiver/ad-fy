#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"


struct TCodeStream{
    const char *source;
    unsigned int position;
};
typedef struct TCodeStream CodeStream;

const char *getToken(CodeStream *stream){
    int i = stream->position;
    int spaceCount = 0;
    while(stream->source[i] == ' '){
        ++spaceCount;
        ++i;
        if(spaceCount == 4){
          stream->position = i;
          return "\t";   
        }
    }
    stream->position = i;
    if(stream->source[i] == '\0'){
        stream->position = i;
        return "";
    }
    if(stream->source[i] == '\t'){
        stream->position = i + 1;
        return "\t";
    }
    if(stream->source[i] == '\n'){
        stream->position = i + 1;
        return "\n";
    }
    if(stream->source[i] == '('){
        stream->position = i + 1;
        return "(";
    }
    if(stream->source[i] == ')'){
        stream->position = i + 1;
        return ")";
    }


    bool found = false;
    while(!found){
      if(
         stream->source[i] == ' '  ||
         stream->source[i] == '('  ||
         stream->source[i] == ')'  ||
         stream->source[i] == '\t' ||
         stream->source[i] == '\n' ||
         stream->source[i] == '\0' 
      )
        found = true;
      else
        ++i;
    }
    int len = i - stream->position;
    char *t = (char *) malloc((len+1) * sizeof(char));
    strncpy(t, stream->source + stream->position, len);    
    t[len] = '\0';
    stream->position = i;
    return t;
}

bool isEndOfCode(CodeStream *ts)
{
    return ts->source[ts->position] == '\0';
}


#endif