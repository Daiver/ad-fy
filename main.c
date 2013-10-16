#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define devel
#include "logging.h"
#include "common.h"

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
    printf("%d ", node.childs_length);
    printf("%s\n", node.name);
    for(int i = 0; i < node.childs_length; i++)
        printTree(node.childs[i], shift + 1);
}

int execute(Node node){
    if(node.childs_length == 0)
        return atoi(node.name);
    if(strcmp(node.name, "*") == 0){
        return execute(node.childs[0]) * execute(node.childs[1]);
    }
    if(strcmp(node.name, "+") == 0){
        return execute(node.childs[0]) + execute(node.childs[1]);
    }
}

//TESTS
void testGetToken(const char *source){
  CodeStream ts = {source, 0};
  const char *tk;
  while( strcmp((tk = getToken(&ts)), ""))
      printf("[%s]\n", tk);
}

void testParseFirst(const char *source){
    TokensStream ts;
    fillTokenStream(&ts, source);
    Node head = parse(&ts, 0);
    printTree(head, 0);
}

void testExecuteFirst(const char *source){
    TokensStream ts;
    fillTokenStream(&ts, source);
    Node head = parse(&ts, 0);
    printTree(head, 0);
    printf("res>%d\n", execute(head));
}


int main(int argc, char **argv)
{

    if(argc > 1){
        printf("Reading from file [%s]...\n", argv[1]);
        const char *src = readFileAsLine(argv[1]);

        printf("{-\n%s\n-}\n", src);
        //testParseFirst(src);
        testExecuteFirst(src);
    }
    //testGetToken("   def    say\n\tdo");
    //testGetToken("(def func (+ 10 11))");
    //printf("def func \n\t+ 10 11\n");
    //printf("def func \n\t+ \n\t\t10 \n\t\t11\n");
    //testParseFirst("def func (+ 10 11)");
    //testParseFirst("def func \n\t+ \n\t\t10 \n\t\t11");
    //printf("def func \n\t+ \n\t\t10 \n\t\t11\n");
    //testParseFirst("def func \n    + 10 11\n    - 2 9");
    //MAIN
    //    testGetToken("   def    say\n\tdo");
    //    testGetToken("(def func (+ 10 11))");
    //    printf("def func \n\t+ 10 11\n");
    //    printf("def func \n\t+ \n\t\t10 \n\t\t11\n");
    //    testParseFirst(" ");
    //    testParseFirst("\n");
    //    testParseFirst("");
    //    testParseFirst("def func \n"
    //		   "\t(+ 10 11)\n");

    //    testParseFirst("def func \n"
    //		   "\t+\n"
    //                 "\t\t10"
    //                   "\n\t\t11");

    //    testParseFirst("def func \n"
    //		   "\t(+ 10 11)\n"
    //		   "\t(- 2 9)\n");
    return 0;
}

