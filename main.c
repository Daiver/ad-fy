#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool char
#define true 1
#define false !true

#define devel
#ifdef devel
#define LOG(a) printf(a);
#else
#define LOG(a)
#endif

struct TNode{
    const char *name;
    unsigned int childs_length;
    struct TNode *childs;
}; 
typedef struct TNode Node;

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

bool isEndOfCode(CodeStream *ts)
{
    return ts->source[ts->position] == '\0';
}

struct TTokenStream{
    CodeStream codeStream;
    const char **tokens;
    unsigned int length;
    unsigned int position;  
};
typedef struct TTokenStream TokensStream;

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
    if(ts->position + shift >= ts->length){
        int diff_length = ts->position + shift - ts->length + 1;
        for(int i = 0; i < diff_length && !isEndOfCode(&ts->codeStream); i++){
            const char *t = getToken(&ts->codeStream);
            ts->length++;
            ts->tokens = (const char **)realloc(ts->tokens, ts->length * sizeof(const char *));
            ts->tokens[ts->length - 1] = t;
        }
    }
    return ts->tokens[ts->position + shift];
}

const char *nextToken(TokensStream *ts){
    const char *res = lookToken(ts, 0);
    ts->position++;
    return res;
}

bool isEndOfStream(TokensStream *ts){
    return ts->position == ts->length;
}

Node parse(TokensStream *ts, int shift){
    LOG("\n=====[parse] begin\n");
    Node res = {0, 0, 0};
    if(isEndOfStream(ts)) 
        return res;
    LOG("\n=====[parse] not eos\n");
    const char *token = nextToken(ts);
    while(strcmp(token, "\t") == 0)
        token = nextToken(ts);

    res.name = token;
    while(!isEndOfStream(ts)){
        token = nextToken(ts);
        if(strcmp(token, ")") == 0) break;
        if(strcmp(token, "\t") == 0) continue;
        bool readGroup = false;  //make it beauty
        if(strcmp(token, "\n") == 0){
            const char *tmp_token = lookToken(ts, 0);
            int shift_count = 0;
            while(strcmp(tmp_token, "\t") == 0){
                shift_count++;
                tmp_token = lookToken(ts, shift_count);
            }
            readGroup = shift_count == shift + 1;
            if(!readGroup)
                break;
        }
        res.childs_length++;
        res.childs = (Node *) realloc(res.childs, res.childs_length * sizeof(Node));
        LOG("\n=====[parse] Checking on ( and readGroup\n");
        if(strcmp(token, "(") == 0 || readGroup){
            LOG("\n=====[parse] true\n");
            res.childs[res.childs_length - 1] = parse(ts, (readGroup ? shift + 1 : shift));
        }
        else{
            LOG("\n=====[parse] false\n");
            res.childs[res.childs_length - 1].name = token;
            res.childs[res.childs_length - 1].childs_length = 0;
        }
    }

    return res;
}

void printTree(Node node, int shift){
    for(int i = 0; i < shift; i++)
        printf("  ");
    printf("%s\n", node.name);
    for(int i = 0; i < node.childs_length; i++)
        printTree(node.childs[i], shift + 1);
}

//TESTS
void testGetToken(const char *source){
  CodeStream ts = {source, 0};
  const char *tk;
  while((tk = getToken(&ts)))
      printf("[%s]\n", tk);
}

void testParseFirst(const char *source){
    TokensStream ts;
    LOG("\n=====BPA=====\n");
    fillTokenStream(&ts, source);
    Node head = parse(&ts, 0);
    printTree(head, 0);
}


//MAIN
int main(int argc, char **argv){
//    testGetToken("   def    say\n\tdo");
//    testGetToken("(def func (+ 10 11))");
//    printf("def func \n\t+ 10 11\n");
//    printf("def func \n\t+ \n\t\t10 \n\t\t11\n");
    testParseFirst("def func (+ 10 11)");

    testParseFirst("def func \n"
		   "\t(+ 10 11)\n");

//    testParseFirst("def func \n"
//		   "\t+\n"
//                 "\t\t10"
//                   "\n\t\t11");

//    testParseFirst("def func \n"
//		   "\t(+ 10 11)\n"
//		   "\t(- 2 9)\n");
    return 0;
}

