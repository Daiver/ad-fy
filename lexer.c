#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "lexer.h"

const char *stopchars[] = {"\0", "\t", "\n", "(", ")"};
const int stopchars_length = sizeof(stopchars)/sizeof(char *);

const char *stopChar(char c, bool *isstopch){
  for(int i = 0; i < stopchars_length; ++i){
    if(*stopchars[i] == c){
        *isstopch = true;
	return stopchars[i];
    }
  }
  isstopch = false;
  return NULL;
}

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
    bool isstopch = false;
    const char *ch = stopChar(stream->source[i], &isstopch);
    if(isstopch){
        stream->position = i + (ch != '\0');
        return ch;
    }
    bool found = false;
    while(!found){
      stopChar(stream->source[i], &isstopch);
      found = isstopch || stream->source[i] == ' ';
      if(!found)
        ++i;
    }
    int len = i - stream->position;
    if(len + stream->position >= strlen(stream->source))
        len = strlen(stream->source) - stream->position - 1;
    char *t = (char *) malloc((len+1) * sizeof(char));
    strncpy(t, stream->source + stream->position, len);    
    t[len] = '\0';
    stream->position = i;
    return t;
}

bool isEndOfCode(CodeStream *ts){
    return ts->source[ts->position] == '\0';
}
