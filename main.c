#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool char
#define true 1
#define false !true

struct TNode
{
    char *name;
    unsigned int childs_length;
    struct TNode *childs;
}; 
typedef struct TNode Node;

struct TTokensStream
{
    const char *source;
    unsigned int position;
};
typedef struct TTokensStream TokensStream;

const char *getToken(TokensStream *stream)
{
    int i = stream->position;
    if(stream->source[i] == '\0'){
        stream->position = i;
        return NULL;
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
    stream->position = i;
    return t;
}


void testGetToken(const char *source){
  TokensStream ts = {source, 0};
  const char *tk;
  while((tk = getToken(&ts)))
      printf("[%s]\n", tk);
}

int main(int argc, char **argv)
{
    testGetToken("   def    say\n\tdo");
    testGetToken("(def func (+ 10 11))");
    return 0;
}

