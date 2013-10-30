#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"
#include "hashtable.h"
#include "parser.h"

ObjectNode *op_Plus(hashtable_t *context, Node *node){
    int res = 0;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res += (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Mul(hashtable_t *context, Node *node){
    int res = 1;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res *= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Minus(hashtable_t *context, Node *node){
    ObjectNode *tmp = execute(context, &node->childs[0]);
    int res = tmp->value;
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res -= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Eq(hashtable_t *context, Node *node){
    ObjectNode *tmp1 = execute(context, &node->childs[0]);
    ObjectNode *tmp2 = execute(context, &node->childs[1]);
    return newObjectNode(NTYPE_BOOL, tmp1->value == tmp2->value);
}


ObjectNode *op_Div(hashtable_t *context, Node *node){
    ObjectNode *tmp = execute(context, &node->childs[0]);
    int res = tmp->value;
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res /= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Help(hashtable_t *context, Node *node){
    printf("\nThis is small lisp like language interpreter by Victor Muzychenko and Kirill Klimov\n");
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Define(hashtable_t *context, Node *node){// FIX IT!
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = execute(context, &node->childs[1]); //(char *)newObjectNode(2, &node->childs[1]);
    ht_del(context, func_name);
    printf("LOLOLOLOLOLLOLOLOL=%d\n", tmp->value);
    ht_set(context, func_name, tmp);
    return tmp;
}

ObjectNode *op_Alias(hashtable_t *context, Node *node){// FIX IT!
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = ht_get(context, node->childs[1].name); //(char *)newObjectNode(2, &node->childs[1]);
    ht_set(context, func_name, tmp);
    return tmp;
}

ObjectNode *op_Fn(hashtable_t *context, Node *node){
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

ObjectNode *op_DefFn(hashtable_t *context, Node *node){
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
    ht_set(context, func_name, tmp);
    return tmp;
}

ObjectNode *op_Quote(hashtable_t *context, Node *node){
    return ht_get(context, node->childs[0].name);
}

ObjectNode *op_If(hashtable_t *context, Node *node){
    ObjectNode *p = execute(context, &node->childs[0]);
    if(p->value != 0)
        return execute(context, &node->childs[1]);
    return execute(context, &node->childs[2]);
}

ObjectNode *op_Import(hashtable_t *context, Node *node){
    import(context, node->childs[0].name);
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Comment(hashtable_t *context, Node *node){
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

ObjectNode *op_Print(hashtable_t *context, Node *node){
    printf("stdout>");
    ObjectNode *res = NULL;
    for(int i = 0; i < node->childs_length; i++){
        res = execute(context, &node->childs[i]);
        printObjectNode(res);
        printf(" ");
    }
    printf("\n");
    if(res == NULL)
        return newObjectNode(NTYPE_NONE, 0);
    return res;
}

ObjectNode *op_List(hashtable_t *context, Node *node){
    ObjectList *res = (ObjectList *)malloc(sizeof(ObjectList));
    res->length = node->childs_length;
    res->items = (ObjectNode *)malloc(sizeof(ObjectNode) * res->length);
    for(int i = 0; i < res->length; i++)
        res->items[i] = *(execute(context, &node->childs[i]));
    return newObjectNode(NTYPE_LIST, res);
}

ObjectNode *op_Elem(hashtable_t *context, Node *node){
    ObjectNode *index = execute(context, &node->childs[0]);
    ObjectNode *res = execute(context, &node->childs[1]);
    if(index->value >= ((ObjectList *)res->value)->length)
        return newObjectNode(NTYPE_NONE, 0);
    return &((ObjectList *)res->value)->items[(int)index->value];
}

ObjectNode *op_Slice(hashtable_t *context, Node *node){
    ObjectNode *start_index = execute(context, &node->childs[0]);
    ObjectNode *end_index = execute(context, &node->childs[1]);
    ObjectNode *li = execute(context, &node->childs[2]);

    ObjectList *tmp = ((ObjectList *)li->value);
    ObjectNode *slice = tmp->items + (int)start_index->value;
    ObjectList *res = newObjectList(end_index->value - start_index->value, slice);
    return newObjectNode(NTYPE_LIST, res);
}

ObjectNode *op_Cons(hashtable_t *context, Node *node){
    ObjectNode *elem = execute(context, &node->childs[0]);
    ObjectNode *li = execute(context, &node->childs[1]);
    ObjectNode *items = malloc(sizeof(ObjectNode) * (((ObjectList *)li->value)->length + 1));// fix fix fix
    for(int i = 0; i < ((ObjectList *)li->value)->length; i++){
        items[i] = ((ObjectList *)li->value)->items[i];
    }
    items[((ObjectList *)li->value)->length] = *elem;
    return newObjectNode(NTYPE_LIST, newObjectList(((ObjectList *)li->value)->length + 1, items));
}

ObjectNode *op_Length(hashtable_t *context, Node *node){
    ObjectNode *res = execute(context, &node->childs[0]);
    return newObjectNode(NTYPE_INT, ((ObjectList *)res->value)->length);
}

typedef ObjectNode*(*OpHandler)(hashtable_t, Node *);

void addOp(hashtable_t *context, char *token, OpHandler handler){
    ht_set(context, token, (char *) newObjectNode(NTYPE_BUILTIN_FUNC, handler));
}

void fillOpTable(hashtable_t *operators){
    addOp(operators, "+", &op_Plus);
    addOp(operators, "-", &op_Minus);
    addOp(operators, "*", &op_Mul);
    addOp(operators, "/", &op_Div);
    addOp(operators, "==", &op_Eq);
    addOp(operators, "help", &op_Help);
    addOp(operators, "define", &op_Define);
    addOp(operators, "lambde", &op_Fn);
    addOp(operators, "alias", &op_Alias);
    addOp(operators, "deffn", &op_DefFn);
    addOp(operators, "print", &op_Print);
    addOp(operators, "import", &op_Import);
    addOp(operators, "if", &op_If);
    addOp(operators, "id", &op_Quote);
    addOp(operators, "length", &op_Length);
    addOp(operators, "cons", &op_Cons);
    addOp(operators, "slice", &op_Slice);
    addOp(operators, "[]", &op_Elem);
    addOp(operators, "list", &op_List);
    addOp(operators, "comment", &op_Comment);
}
