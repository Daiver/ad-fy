#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"
#include "parser.h"
#include "context.h"
#include "builtins.h"
#include "logging.h"

ObjectNode *op_Plus(Context *context, Node *node){
    int res = 0;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res += (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Mul(Context *context, Node *node){
    int res = 1;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res *= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Minus(Context *context, Node *node){
    ObjectNode *tmp = execute(context, &node->childs[0]);
    int res = tmp->value;
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res -= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Eq(Context *context, Node *node){
    ObjectNode *tmp1 = execute(context, &node->childs[0]);
    ObjectNode *tmp2 = execute(context, &node->childs[1]);
    return newObjectNode(NTYPE_BOOL, tmp1->value == tmp2->value);
}


ObjectNode *op_Div(Context *context, Node *node){
    ObjectNode *tmp = execute(context, &node->childs[0]);
    int res = tmp->value;
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res /= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

ObjectNode *op_Help(Context *context, Node *node){
    printf("\nThis is small lisp like language interpreter by Victor Muzychenko and Kirill Klimov\n");
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Define(Context *context, Node *node){// FIX IT!
    LOG("op_Define", "start");
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = execute(context, &node->childs[1]); //(char *)newObjectNode(2, &node->childs[1]);
    context_set(context, func_name, tmp, true);
    LOG("op_Define", "end");
    return tmp;
}

ObjectNode *op_Alias(Context *context, Node *node){// FIX IT!
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = context_get(context, node->childs[1].name); //(char *)newObjectNode(2, &node->childs[1]);
    context_set(context, func_name, tmp, true);
    return tmp;
}

ObjectNode *op_Fn(Context *context, Node *node){
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

ObjectNode *op_DefFn(Context *context, Node *node){
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
    context_set(context, func_name, tmp, true);
    return tmp;
}

ObjectNode *op_Quote(Context *context, Node *node){
    return context_get(context, node->childs[0].name);
}

ObjectNode *op_If(Context *context, Node *node){
    ObjectNode *p = execute(context, &node->childs[0]);
    if(p->value != 0)
        return execute(context, &node->childs[1]);
    return execute(context, &node->childs[2]);
}

ObjectNode *op_Import(Context *context, Node *node){
    import(context, node->childs[0].name);
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Comment(Context *context, Node *node){
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

ObjectNode *op_Print(Context *context, Node *node){
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

ObjectNode *op_List(Context *context, Node *node){
    ObjectList *res = (ObjectList *)malloc(sizeof(ObjectList));
    res->length = node->childs_length;
    res->items = (ObjectNode *)malloc(sizeof(ObjectNode) * res->length);
    for(int i = 0; i < res->length; i++)
        res->items[i] = *(execute(context, &node->childs[i]));
    return newObjectNode(NTYPE_LIST, res);
}

ObjectNode *op_Elem(Context *context, Node *node){
    ObjectNode *index = execute(context, &node->childs[0]);
    ObjectNode *res = execute(context, &node->childs[1]);
    if(index->value >= ((ObjectList *)res->value)->length)
        return newObjectNode(NTYPE_NONE, 0);
    return &((ObjectList *)res->value)->items[(int)index->value];
}

ObjectNode *op_Slice(Context *context, Node *node){
    ObjectNode *start_index = execute(context, &node->childs[0]);
    ObjectNode *end_index = execute(context, &node->childs[1]);
    ObjectNode *li = execute(context, &node->childs[2]);

    ObjectList *tmp = ((ObjectList *)li->value);
    ObjectNode *slice = tmp->items + (int)start_index->value;
    ObjectList *res = newObjectList(end_index->value - start_index->value, slice);
    return newObjectNode(NTYPE_LIST, res);
}

ObjectNode *op_Cons(Context *context, Node *node){
    ObjectNode *elem = execute(context, &node->childs[0]);
    ObjectNode *li = execute(context, &node->childs[1]);
    ObjectNode *items = malloc(sizeof(ObjectNode) * (((ObjectList *)li->value)->length + 1));// fix fix fix
    for(int i = 0; i < ((ObjectList *)li->value)->length; i++){
        items[i] = ((ObjectList *)li->value)->items[i];
    }
    items[((ObjectList *)li->value)->length] = *elem;
    return newObjectNode(NTYPE_LIST, newObjectList(((ObjectList *)li->value)->length + 1, items));
}

ObjectNode *op_Length(Context *context, Node *node){
    ObjectNode *res = execute(context, &node->childs[0]);
    return newObjectNode(NTYPE_INT, ((ObjectList *)res->value)->length);
}

void addOp(Context *context, char *token, OpHandler handler){
    context_set(context, token, (void *) newObjectNode(NTYPE_BUILTIN_FUNC, handler), true);
}

void fillOpTable(Context *context){
    addOp(context, "+", &op_Plus);
    addOp(context, "-", &op_Minus);
    addOp(context, "*", &op_Mul);
    addOp(context, "/", &op_Div);
    addOp(context, "==", &op_Eq);
    addOp(context, "help", &op_Help);
    addOp(context, "define", &op_Define);
    addOp(context, "lambde", &op_Fn);
    addOp(context, "alias", &op_Alias);
    addOp(context, "deffn", &op_DefFn);
    addOp(context, "print", &op_Print);
    addOp(context, "import", &op_Import);
    addOp(context, "if", &op_If);
    addOp(context, "id", &op_Quote);
    addOp(context, "length", &op_Length);
    addOp(context, "cons", &op_Cons);
    addOp(context, "slice", &op_Slice);
    addOp(context, "[]", &op_Elem);
    addOp(context, "list", &op_List);
    addOp(context, "comment", &op_Comment);
}
