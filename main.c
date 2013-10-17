#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define devel
#include "logging.h"
#include "common.h"
#include "hashtable.h"

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

bool isDigit(const char *s){
    const char *str = s;
    if(*str == '-'){
        str++;
        if(*str == '\0')
            return false;
    }
    while (*str != '\0'){
        if (*str < 48 || *str > 57) 
            return false;
        str++;
    }
    return true;
}

struct TObjectNode{
    //types :
    //0 - none
    //1 - built-in function 
    //2 - node
    //3 - function
    //
    //101 - int
    unsigned char type;
    void *value;
};
typedef struct TObjectNode ObjectNode;

ObjectNode *newObjectNode(unsigned char type, void *value){
    ObjectNode *obj = (ObjectNode *) malloc( sizeof(ObjectNode));
    obj->type = type;
    obj->value = value;
    return obj;
}

struct TFunctionObj{
    int args_length;
    const char *prefix;
    char **args;
    Node *node;
};
typedef struct TFunctionObj FunctionObj;

//FunctionObj *newFunctionObj()

ObjectNode *execute(hashtable_t *hashtable, Node *node){
    ObjectNode *res = newObjectNode(0, 0);
    if(isDigit(node->name)){
        res->type = 101;
        res->value = (void *)atoi(node->name);
    }
    else{
        ObjectNode *obj = ht_get(hashtable, node->name);
        if(obj == NULL){
            printf("EXECTE ERROR obj [%s] does not exists\n", node->name);
            return res;
        }
        if(obj->type == 1){
            ObjectNode *(*fp)(hashtable_t *hashtable, Node *node) = obj->value;
            res = fp(hashtable, node);
        }else if(obj->type == 2){
            return execute(hashtable, obj->value);
        }else if(obj->type == 3){
            FunctionObj *foo = (FunctionObj *)obj->value;
            for(int i = 0; i < foo->args_length; i++){
                ht_set(hashtable, foo->args[i], execute(hashtable, &node->childs[i]));
            }
            res = execute(hashtable, foo->node);
            for(int i = 0; i < foo->args_length; i++){
                ht_del(hashtable, foo->args[i]);
            }
           
            return res;
        }else if(obj->type == 101){
            return obj;
        }
        else{
            printf("EXECTE ERROR type [%d] does not exists\n", obj->type);
            return res;
        }
    }
    return res;
}

ObjectNode *op_Plus(hashtable_t *hashtable, Node *node){
    int res = 0;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(hashtable, &node->childs[i]);
        res += (int)tmp->value;
    }
    return newObjectNode(101, res);
}

ObjectNode *op_Mul(hashtable_t *hashtable, Node *node){
    int res = 1;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(hashtable, &node->childs[i]);
        res *= (int)tmp->value;
    }
    return newObjectNode(101, res);
}

ObjectNode *op_Minus(hashtable_t *hashtable, Node *node){
    ObjectNode *tmp = execute(hashtable, &node->childs[0]);
    int res = tmp->value;
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(hashtable, &node->childs[i]);
        res -= (int)tmp->value;
    }
    return newObjectNode(101, res);
}

ObjectNode *op_Div(hashtable_t *hashtable, Node *node){
    ObjectNode *tmp = execute(hashtable, &node->childs[0]);
    int res = tmp->value;
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(hashtable, &node->childs[i]);
        res /= (int)tmp->value;
    }
    return newObjectNode(101, res);
}

ObjectNode *op_Help(hashtable_t *hashtable, Node *node){
    printf("\nThis is small lisp like language interpreter by Victor Muzychenko and Kirill Klimov\n");
    return newObjectNode(0, 0);
}

ObjectNode *op_Define(hashtable_t *hashtable, Node *node){// FIX IT!
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = execute(hashtable, &node->childs[1]); //(char *)newObjectNode(2, &node->childs[1]);
    ht_set(hashtable, func_name, tmp);
    return tmp;
}

ObjectNode *op_Fn(hashtable_t *hashtable, Node *node){
    FunctionObj *fo = (FunctionObj *)malloc(sizeof(FunctionObj));
    fo->args_length = node->childs[0].childs_length;
    fo->args = (char **)malloc(sizeof(char *) * fo->args_length);
    for(int i = 0; i < fo->args_length; i++){
        fo->args[i] = node->childs[0].childs[i].name;
    }
    fo->node = &node->childs[1];
    return newObjectNode(3, (void *)fo);
}


ObjectNode *op_DefFn(hashtable_t *hashtable, Node *node){
    const char *func_name = node->childs[0].name; 
    FunctionObj *fo = (FunctionObj *)malloc(sizeof(FunctionObj));
    fo->args_length = node->childs[1].childs_length;
    fo->args = (char **)malloc(sizeof(char *) * fo->args_length);
    for(int i = 0; i < fo->args_length; i++){
        fo->args[i] = node->childs[1].childs[i].name;
    }
    fo->node = &node->childs[2];
    ObjectNode *tmp = newObjectNode(3, (void *)fo);
    ht_set(hashtable, func_name, tmp);
    return tmp;
}

void fillOpTable(hashtable_t *hashtable){
    ht_set(hashtable, "+", (char *)newObjectNode(1, &op_Plus));
    ht_set(hashtable, "-", (char *)newObjectNode(1, &op_Minus));
    ht_set(hashtable, "*", (char *)newObjectNode(1, &op_Mul));
    ht_set(hashtable, "/", (char *)newObjectNode(1, &op_Div));
    ht_set(hashtable, "help", (char *)newObjectNode(1, &op_Help));
    ht_set(hashtable, "define", (char *)newObjectNode(1, &op_Define));
    ht_set(hashtable, "lambda", (char *)newObjectNode(1, &op_Fn));
    ht_set(hashtable, "deffn", (char *)newObjectNode(1, &op_DefFn));
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
    hashtable_t *hashtable = ht_create( 65536 ); 
    fillOpTable(hashtable);
    while(!isEndOfStream(&ts)){
        Node head = parse(&ts, 0);
        printTree(head, 0);
        ObjectNode *node = execute(hashtable, &head);
        if(node->type == 101){
            printf("res>%d\n", node->value);
        }
    }
}


int main(int argc, char **argv)
{

    if(argc > 1){
        printf("Reading from file [%s]...\n", argv[1]);
        const char *src = readFileAsLine(argv[1]);

        printf("{-\n%s\n-}\n", src);
        testExecuteFirst(src);
    }
    return 0;
}

