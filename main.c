#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define devel
#include "logging.h"
#include "common.h"
#include "hashtable.h"
#include "lexer.h"
#include "parser.h"

struct TObjectNode{
    //types :
    //0 - none
    //1 - built-in function 
    //2 - node
    //3 - function
    //
    //101 - int
    //105 - bool
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
    int node_length;
    Node **nodes;
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
        }else if(obj->type == 3){// MAKE ITBETTER. PLEASE ='(
            FunctionObj *foo = (FunctionObj *)obj->value;
            ObjectNode **objs = malloc(sizeof(ObjectNode *) * node->childs_length);
            for(int i = 0; i < node->childs_length; i++){
                objs[i] = execute(hashtable, &node->childs[i]);
            }
            void **backup = malloc(sizeof(void *) * foo->args_length);
            for(int i = 0; i < foo->args_length; i++){
                void *tmp = ht_get(hashtable, foo->args[i]);
                if(tmp != NULL){
                    backup[i] = tmp;
                    ht_del(hashtable, foo->args[i]);
                }
                else
                    backup[i] = NULL;
                ht_set(hashtable, foo->args[i], objs[i]);
                //ht_set(hashtable, foo->args[i], execute(hashtable, &node->childs[i]));
            }
            for(int i = 0; i < foo->node_length; i++){
                res = execute(hashtable, foo->nodes[i]);
            }
            for(int i = 0; i < foo->args_length; i++){
                ht_del(hashtable, foo->args[i]);
                if(backup[i] != NULL){
                    ht_set(hashtable, foo->args[i], backup[i]);
                }
            }
            free(backup); 
            free(objs);
            return res;
        }else if(obj->type > 100){
            return obj;
        }
        else{
            printf("EXECTE ERROR type [%d] does not exists\n", obj->type);
            return res;
        }
    }
    return res;
}

void import(hashtable_t *hashtable, const char *fname){
    const char *source = readFileAsLine(fname);
    TokensStream ts;
    fillTokenStream(&ts, source);
    while(!isEndOfStream(&ts)){
        Node head = parse(&ts, 0);
        ObjectNode *node = execute(hashtable, &head);
    }
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

ObjectNode *op_Eq(hashtable_t *hashtable, Node *node){
    ObjectNode *tmp1 = execute(hashtable, &node->childs[0]);
    ObjectNode *tmp2 = execute(hashtable, &node->childs[1]);
    return newObjectNode(105, tmp1->value == tmp2->value);
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

ObjectNode *op_Alias(hashtable_t *hashtable, Node *node){// FIX IT!
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = ht_get(hashtable, node->childs[1].name); //(char *)newObjectNode(2, &node->childs[1]);
    ht_set(hashtable, func_name, tmp);
    return tmp;
}



ObjectNode *op_Fn(hashtable_t *hashtable, Node *node){
    FunctionObj *fo = (FunctionObj *)malloc(sizeof(FunctionObj));
    int starts_with = 0;
    if(strcmp(node->childs[0].name, "args") == 0){
        fo->args_length = node->childs[0].childs_length;
        fo->args = (char **)malloc(sizeof(char *) * fo->args_length);
        for(int i = 0; i < fo->args_length; i++){
            fo->args[i] = node->childs[0].childs[i].name;
        }
        starts_with = 1;
    }
    else{
        fo->args_length = 0;
        fo->args = 0;
    }
    fo->node_length = node->childs_length - starts_with;
    fo->nodes = malloc(sizeof(Node *) * fo->node_length);
    for(int i = starts_with; i < node->childs_length; i++)
        fo->nodes[i - starts_with] = &node->childs[i];
    return newObjectNode(3, (void *)fo);
}


ObjectNode *op_DefFn(hashtable_t *hashtable, Node *node){
    const char *func_name = node->childs[0].name; 
    FunctionObj *fo = (FunctionObj *)malloc(sizeof(FunctionObj));
    int starts_with = 1;
    if(strcmp(node->childs[1].name, "args") == 0){
        fo->args_length = node->childs[1].childs_length;
        fo->args = (char **)malloc(sizeof(char *) * fo->args_length);
        for(int i = 0; i < fo->args_length; i++){
            fo->args[i] = node->childs[1].childs[i].name;
        }
        starts_with = 2;
    }
    else{
        fo->args_length = 0;
        fo->args = 0;
    }
    fo->node_length = node->childs_length - starts_with;
    fo->nodes = malloc(sizeof(Node *) * fo->node_length);
    for(int i = starts_with; i < node->childs_length; i++)
        fo->nodes[i - starts_with] = &node->childs[i];
    ObjectNode *tmp = newObjectNode(3, (void *)fo);
    ht_set(hashtable, func_name, tmp);
    return tmp;
}

ObjectNode *op_Quote(hashtable_t *hashtable, Node *node){
    return ht_get(hashtable, node->childs[0].name);
}

ObjectNode *op_If(hashtable_t *hashtable, Node *node){
    ObjectNode *p = execute(hashtable, &node->childs[0]);
    if(p->value != 0)
        return execute(hashtable, &node->childs[1]);
    return execute(hashtable, &node->childs[2]);
}

ObjectNode *op_Import(hashtable_t *hashtable, Node *node){
    import(hashtable, node->childs[0].name);
    return newObjectNode(0, 0);
}

ObjectNode *op_Comment(hashtable_t *hashtable, Node *node){
    return newObjectNode(0, 0);
}

ObjectNode *op_Print(hashtable_t *hashtable, Node *node){
    ObjectNode *res = execute(hashtable, &node->childs[0]);
    if(res->type == 101){
        printf("stdout>%d\n", res->value);
    }
    return newObjectNode(0, 0);
}


void fillOpTable(hashtable_t *hashtable){
    ht_set(hashtable, "+", (char *)newObjectNode(1, &op_Plus));
    ht_set(hashtable, "-", (char *)newObjectNode(1, &op_Minus));
    ht_set(hashtable, "*", (char *)newObjectNode(1, &op_Mul));
    ht_set(hashtable, "/", (char *)newObjectNode(1, &op_Div));
    ht_set(hashtable, "==", (char *)newObjectNode(1, &op_Eq));
    ht_set(hashtable, "help", (char *)newObjectNode(1, &op_Help));
    ht_set(hashtable, "define", (char *)newObjectNode(1, &op_Define));
    ht_set(hashtable, "lambda", (char *)newObjectNode(1, &op_Fn));
    ht_set(hashtable, "alias", (char *)newObjectNode(1, &op_Alias));
    ht_set(hashtable, "deffn", (char *)newObjectNode(1, &op_DefFn));
    ht_set(hashtable, "id", (char *)newObjectNode(1, &op_Quote));
    ht_set(hashtable, "if", (char *)newObjectNode(1, &op_If));
    ht_set(hashtable, "import", (char *)newObjectNode(1, &op_Import));
    ht_set(hashtable, "print", (char *)newObjectNode(1, &op_Print));
    ht_set(hashtable, "comment", (char *)newObjectNode(1, &op_Comment));
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

void testExecuteSecond(const char *source){
    TokensStream ts;
    fillTokenStream(&ts, source);
    hashtable_t *hashtable = ht_create( 65536 ); 
    fillOpTable(hashtable);
    import(hashtable, "stl.x");
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
        testExecuteSecond(src);
    }
    return 0;
}

