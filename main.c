#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool char
#define true 1
#define false !true

struct TNode
{
    const char *name;
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

bool isEndOfStream(TokensStream *ts)
{
    return ts->source[ts->position] == '\0';
}

Node parse(TokensStream *ts){
    const char *token = getToken(ts);
    Node res = {0, 0, 0};

    res.name = token;
    while(!isEndOfStream(ts)){
        token = getToken(ts);
        if(strcmp(token, ")") == 0) break;
        res.childs_length++;
        res.childs = (Node *)realloc(res.childs, res.childs_length * sizeof(Node));
        if(strcmp(token, "(") != 0){
            res.childs[res.childs_length - 1].name = token;
            res.childs[res.childs_length - 1].childs_length = 0;
        }
        else
            res.childs[res.childs_length - 1] = parse(ts);
    }

    return res;
}

void printTree(Node node, int shift)
{
    for(int i = 0; i < shift; i++)
        printf("  ");
    printf("%s\n", node.name);
    for(int i = 0; i < node.childs_length; i++)
        printTree(node.childs[i], shift + 1);
}

//TESTS
void testGetToken(const char *source){
  TokensStream ts = {source, 0};
  const char *tk;
  while((tk = getToken(&ts)))
      printf("[%s]\n", tk);
}

void testParseFirst(const char *source)
{
    TokensStream ts = {source, 0};
    Node head = parse(&ts);
    printTree(head, 0);
}

int main(int argc, char **argv)
{
    //testGetToken("   def    say\n\tdo");
    //testGetToken("(def func (+ 10 11))");
    testParseFirst("def func (+ 10 11)");
    return 0;
}

