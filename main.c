#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "common.h"
#include "hashtable.h"
#include "lexer.h"
#include "parser.h"

#define NTYPE_NONE 0
#define NTYPE_BUILTIN_FUNC 1
#define NTYPE_NODE 2
#define NTYPE_FUNC 3
#define NTYPE_INT 101
#define NTYPE_BOOL 105
#define NTYPE_LIST 110

struct TObjectNode{
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

struct TObjectList{
    int length;
    ObjectNode *items;
};
typedef struct TObjectList ObjectList;

ObjectList *newObjectList(int length, ObjectNode *items){
    ObjectList *res = (ObjectList *)malloc(sizeof(ObjectList));
    res->length = length;
    res->items = items;
    return res;
}

ObjectNode *execute(hashtable_t *hashtable, Node *node){
    ObjectNode *res = newObjectNode(NTYPE_NONE, 0);
    //printf("[%s]\n", node->name);
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
    TokenStream ts;
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
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Mul(hashtable_t *hashtable, Node *node){
    int res = 1;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(hashtable, &node->childs[i]);
        res *= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Minus(hashtable_t *hashtable, Node *node){
    ObjectNode *tmp = execute(hashtable, &node->childs[0]);
    int res = tmp->value;
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(hashtable, &node->childs[i]);
        res -= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Eq(hashtable_t *hashtable, Node *node){
    ObjectNode *tmp1 = execute(hashtable, &node->childs[0]);
    ObjectNode *tmp2 = execute(hashtable, &node->childs[1]);
    return newObjectNode(NTYPE_BOOL, tmp1->value == tmp2->value);
}


ObjectNode *op_Div(hashtable_t *hashtable, Node *node){
    ObjectNode *tmp = execute(hashtable, &node->childs[0]);
    int res = tmp->value;
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(hashtable, &node->childs[i]);
        res /= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Help(hashtable_t *hashtable, Node *node){
    printf("\nThis is small lisp like language interpreter by Victor Muzychenko and Kirill Klimov\n");
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Define(hashtable_t *hashtable, Node *node){// FIX IT!
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = execute(hashtable, &node->childs[1]); //(char *)newObjectNode(2, &node->childs[1]);
    ht_del(hashtable, func_name);
    printf("LOLOLOLOLOLLOLOLOL=%d\n", tmp->value);
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
    return newObjectNode(NTYPE_FUNC, (void *)fo);
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
    ObjectNode *tmp = newObjectNode(NTYPE_FUNC, (void *)fo);
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
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Comment(hashtable_t *hashtable, Node *node){
    return newObjectNode(NTYPE_NONE, 0);
}

void printObjectNode(ObjectNode *obj){
    if(obj->type == 101){
        printf("%d", obj->value);
    }
    if(obj->type == 110){
        ObjectList *li = (ObjectList *)obj->value;
        printf("[");
        for(int i = 0; i < li->length; i++){
            printObjectNode(&li->items[i]);
            if(i < li->length - 1)
                printf(" ");
        }
        printf("]");
    }
}

ObjectNode *op_Print(hashtable_t *hashtable, Node *node){
    printf("stdout>");
    ObjectNode *res = NULL;
    for(int i = 0; i < node->childs_length; i++){
        res = execute(hashtable, &node->childs[i]);
        printObjectNode(res);
        printf(" ");
    }
    printf("\n");
    if(res == NULL)
        return newObjectNode(NTYPE_NONE, 0);
    return res;
}

ObjectNode *op_List(hashtable_t *hashtable, Node *node){
    ObjectList *res = (ObjectList *)malloc(sizeof(ObjectList));
    res->length = node->childs_length;
    res->items = (ObjectNode *)malloc(sizeof(ObjectNode) * res->length);
    for(int i = 0; i < res->length; i++)
        res->items[i] = *(execute(hashtable, &node->childs[i]));
    return newObjectNode(NTYPE_LIST, res);
}

ObjectNode *op_Elem(hashtable_t *hashtable, Node *node){
    ObjectNode *index = execute(hashtable, &node->childs[0]);
    ObjectNode *res = execute(hashtable, &node->childs[1]);
    if(index->value >= ((ObjectList *)res->value)->length)
        return newObjectNode(NTYPE_NONE, 0);
    return &((ObjectList *)res->value)->items[(int)index->value];
}

ObjectNode *op_Slice(hashtable_t *hashtable, Node *node){
    ObjectNode *start_index = execute(hashtable, &node->childs[0]);
    ObjectNode *end_index = execute(hashtable, &node->childs[1]);
    ObjectNode *li = execute(hashtable, &node->childs[2]);

    ObjectList *tmp = ((ObjectList *)li->value);
    ObjectNode *slice = tmp->items + (int)start_index->value;
    ObjectList *res = newObjectList(end_index->value - start_index->value, slice);
    return newObjectNode(NTYPE_LIST, res);
}

ObjectNode *op_Cons(hashtable_t *hashtable, Node *node){
    ObjectNode *elem = execute(hashtable, &node->childs[0]);
    ObjectNode *li = execute(hashtable, &node->childs[1]);
    ObjectNode *items = malloc(sizeof(ObjectNode) * (((ObjectList *)li->value)->length + 1));// fix fix fix
    for(int i = 0; i < ((ObjectList *)li->value)->length; i++){
        items[i] = ((ObjectList *)li->value)->items[i];
    }
    items[((ObjectList *)li->value)->length] = *elem;
    return newObjectNode(NTYPE_LIST, newObjectList(((ObjectList *)li->value)->length + 1, items));
}

ObjectNode *op_Length(hashtable_t *hashtable, Node *node){
    ObjectNode *res = execute(hashtable, &node->childs[0]);
    return newObjectNode(NTYPE_INT, ((ObjectList *)res->value)->length);
}

//ObjectNode *op_Block(hashtable_t *hashtable, Node *node){
//    ObjectNode *res = newObjectNode(0, 0);
//    for(int i = 0; i < )
//}

// keep it less than 30
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
    ht_set(hashtable, "list", (char *)newObjectNode(1, &op_List));
    ht_set(hashtable, "[]", (char *)newObjectNode(1, &op_Elem));
    ht_set(hashtable, "slice", (char *)newObjectNode(1, &op_Slice));
    ht_set(hashtable, "cons", (char *)newObjectNode(1, &op_Cons));
    ht_set(hashtable, "length", (char *)newObjectNode(1, &op_Length));
}

void testExecuteSecond(const char *source){
    TokenStream ts;
    fillTokenStream(&ts, source);
    hashtable_t *hashtable = ht_create( 65536 ); 
    fillOpTable(hashtable);
    import(hashtable, "stl.x");
    while(!isEndOfStream(&ts)){
        Node head = parse(&ts, 0);
        printTree(head, 0);
        ObjectNode *node = execute(hashtable, &head);
        printf("res>");
        printObjectNode(node);
        printf("\n");
    }
}

int main(int argc, char **argv){
    if(argc > 1){
        printf("Reading from file [%s]...\n", argv[1]);
        const char *src = readFileAsLine(argv[1]);
        printf("{-\n%s\n-}\n", src);
        testExecuteSecond(src);
    }
    else{
        printf("USAGE ./main source_file");
    }
    return 0;
}

