#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "executor.h"
#include "parser.h"
#include "context.h"
#include "builtins.h"

ObjectNode *newObjectNode(unsigned char type, void *value){
    ObjectNode *obj = (ObjectNode *) malloc( sizeof(ObjectNode));
    obj->type = type;
    obj->value = value;
    return obj;
}

ObjectList *newObjectList(int length, ObjectNode *items){
    ObjectList *res = (ObjectList *)malloc(sizeof(ObjectList));
    res->length = length;
    res->items = items;
    return res;
}

ObjectNode *listFromString(const char *s){
    ObjectList *res = (ObjectList *)malloc(sizeof(ObjectList));
    int length = strlen(s);
    res->length = length - 2;
    res->items = (ObjectNode *)malloc(sizeof(ObjectNode) * res->length);
    for(int i = 1; i < length - 1; i++){
        res->items[i - 1] = *newObjectNode(NTYPE_CHAR, (void *)s[i]);
    }
    //res->items = items;
    return newObjectNode(NTYPE_STRING, res); 
}

ObjectNode *setNumType(bool isDouble, double res){ // i dont know how call it better
    if(isDouble){
        double *tmp = malloc(sizeof(double));
        *tmp = res;
        return newObjectNode(NTYPE_DOUBLE, tmp);
    }
    return newObjectNode(NTYPE_INT, (int)res);
}

ObjectNode *op_Plus
    (ExecuteHandler execute, Context *context, Node *node){
    double res = 0;
    bool isDouble = false;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        if(tmp->type == NTYPE_DOUBLE){//TODO Make it better
            isDouble = true;
            res += *(double *)tmp->value;
        }
        else
            res += (int)tmp->value;
    }
    return setNumType(isDouble, res);
}

ObjectNode *op_Mul
    (ExecuteHandler execute, Context *context, Node *node){
    bool isDouble = false;
    double res = 1;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        if(tmp->type == NTYPE_DOUBLE){
            isDouble = true;
            res *= *(double *)tmp->value;
        }
        else
            res *= (int)tmp->value;
    }
    return setNumType(isDouble, res);
}

ObjectNode *op_Minus
    (ExecuteHandler execute, Context *context, Node *node){
    bool isDouble = false;
    ObjectNode *tmp = execute(context, &node->childs[0]);
    double res = 0;
    if(tmp->type == NTYPE_DOUBLE){
        isDouble = true;
        res = *(double *)tmp->value;
    }
    else{
        res = (int)tmp->value;
    }
    for(int i = 1; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        if(tmp->type == NTYPE_DOUBLE){
            isDouble = true;
            res -= *(double *)tmp->value;
        }
        else{
            res -= (int)tmp->value;
        }
    }
    return setNumType(isDouble, res);
}

ObjectNode *op_Eq
    (ExecuteHandler execute, Context *context, Node *node){//make it better
    ObjectNode *tmp1 = execute(context, &node->childs[0]);
    ObjectNode *tmp2 = execute(context, &node->childs[1]);
    bool res;
    if((tmp1->type == NTYPE_INT && tmp2->type == NTYPE_DOUBLE) ||
        (tmp1->type == NTYPE_DOUBLE && tmp2->type == NTYPE_INT) ||
        (tmp1->type == NTYPE_DOUBLE && tmp2->type == NTYPE_DOUBLE)){
        double r1 = tmp1->type == NTYPE_DOUBLE ? *(double *)tmp1->value : (int)tmp1->value;
        double r2 = tmp2->type == NTYPE_DOUBLE ? *(double *)tmp2->value : (int)tmp2->value;
        res = fabs(r1 - r2) < 0.000001; // bad. maybe it useless
    }
    else{
        res = tmp1->value == tmp2->value;
    }
    return newObjectNode(NTYPE_BOOL, res);
}


ObjectNode *op_Div
    (ExecuteHandler execute, Context *context, Node *node){
    ObjectNode *tmp = execute(context, &node->childs[0]);
    bool isDouble = false;
    double res = 0;
    if(tmp->type == NTYPE_DOUBLE){
        isDouble = true;
        res = *(double *)tmp->value;
    }
    else{
        res = (int)tmp->value;
    }
    for(int i = 1; i < node->childs_length; i++){
        tmp = execute(context, &node->childs[i]);
        if(tmp->type == NTYPE_DOUBLE){
            isDouble = true;
            res /= *(double *)tmp->value;
        }
        else{
            res /= (int)tmp->value;
        }
    }
    return setNumType(isDouble, res);

}

ObjectNode *op_Help
    (ExecuteHandler execute, Context *context, Node *node){
    printf("\nThis is small lisp like language interpreter by Victor Muzychenko and Kirill Klimov\n");
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Define
    (ExecuteHandler execute, Context *context, Node *node){
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = execute(context, &node->childs[1]); //(char *)newObjectNode(2, &node->childs[1]);
    context_set(context, func_name, tmp);
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Alias
    (ExecuteHandler execute, Context *context, Node *node){
    const char *func_name = node->childs[0].name; 
    ObjectNode *tmp = context_get(context, node->childs[1].name); //(char *)newObjectNode(2, &node->childs[1]);
    context_set(context, func_name, tmp);
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Fn
    (ExecuteHandler execute, Context *context, Node *node){
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
    fo->context = context;
    return newObjectNode(NTYPE_FUNC, (void *)fo);
}

ObjectNode *op_DefFn
    (ExecuteHandler execute, Context *context, Node *node){
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
    fo->context = context;
    fo->node_length = node->childs_length - starts_with;
    fo->nodes = malloc(sizeof(Node *) * fo->node_length);
    for(int i = starts_with; i < node->childs_length; i++)
        fo->nodes[i - starts_with] = &node->childs[i];
    ObjectNode *tmp = newObjectNode(NTYPE_FUNC, (void *)fo);
    context_set(context, func_name, tmp);
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Quote
    (ExecuteHandler execute, Context *context, Node *node){
    return context_get(context, node->childs[0].name);
}

ObjectNode *op_If
    (ExecuteHandler execute, Context *context, Node *node){
    ObjectNode *p = execute(context, &node->childs[0]);
    if(p->value != 0)
        return execute(context, &node->childs[1]);
    return execute(context, &node->childs[2]);
}

ObjectNode *op_Import
    (ExecuteHandler execute, Context *context, Node *node){
    import(context, node->childs[0].name);
    return newObjectNode(NTYPE_NONE, 0);
}

ObjectNode *op_Comment
    (ExecuteHandler execute, Context *context, Node *node){
    return newObjectNode(NTYPE_NONE, 0);
}

void printObjectNode(ObjectNode *obj){
    if(obj->type == NTYPE_INT){
        printf("%d", obj->value);
    }    
    if(obj->type == NTYPE_DOUBLE){
        printf("%f", *(double *)obj->value);
    }
    if(obj->type == NTYPE_BOOL){
        printf("%s", obj->value ? "True" : "False");
    }
    //printf(">%d %d\n", obj->type, obj->value);
    if(obj->type == NTYPE_CHAR){
        printf("%c", (char)obj->value);
    }
    if(obj->type == NTYPE_STRING){
        ObjectList *li = (ObjectList *)obj->value;
        //printf("length %d\n", li->length);
        printf("'");
        for(int i = 0; i < li->length; i++){
            printObjectNode(&li->items[i]);
        }
        printf("'");
    }

    if(obj->type == NTYPE_LIST){
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

ObjectNode *op_Print
    (ExecuteHandler execute, Context *context, Node *node){
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

ObjectNode *op_List
    (ExecuteHandler execute, Context *context, Node *node){
    ObjectList *res = (ObjectList *)malloc(sizeof(ObjectList));
    res->length = node->childs_length;
    res->items = (ObjectNode *)malloc(sizeof(ObjectNode) * res->length);
    for(int i = 0; i < res->length; i++)
        res->items[i] = *(execute(context, &node->childs[i]));
    return newObjectNode(NTYPE_LIST, res);
}

ObjectNode *op_Elem
    (ExecuteHandler execute, Context *context, Node *node){
    ObjectNode *index = execute(context, &node->childs[0]);
    ObjectNode *res = execute(context, &node->childs[1]);
    if(index->value >= ((ObjectList *)res->value)->length)
        return newObjectNode(NTYPE_NONE, 0);
    return &((ObjectList *)res->value)->items[(int)index->value];
}

ObjectNode *op_Slice
    (ExecuteHandler execute, Context *context, Node *node){
    ObjectNode *start_index = execute(context, &node->childs[0]);
    ObjectNode *end_index = execute(context, &node->childs[1]);
    ObjectNode *li = execute(context, &node->childs[2]);

    ObjectList *tmp = ((ObjectList *)li->value);
    ObjectNode *slice = tmp->items + (int)start_index->value;
    ObjectList *res = newObjectList(end_index->value - start_index->value, slice);
    return newObjectNode(li->type, res);
}

ObjectNode *op_Cons
    (ExecuteHandler execute, Context *context, Node *node){
    ObjectNode *elem = execute(context, &node->childs[0]);
    ObjectNode *li = execute(context, &node->childs[1]);
    ObjectNode *items = malloc(sizeof(ObjectNode) * (((ObjectList *)li->value)->length + 1));// fix fix fix
    for(int i = 0; i < ((ObjectList *)li->value)->length; i++){
        items[i] = ((ObjectList *)li->value)->items[i];
    }
    items[((ObjectList *)li->value)->length] = *elem;
    return newObjectNode((elem->type == 0 || elem->type == NTYPE_CHAR) && li->type == NTYPE_STRING ? NTYPE_STRING : NTYPE_LIST, 
        newObjectList(((ObjectList *)li->value)->length + 1, items));
}

ObjectNode *op_Length
    (ExecuteHandler execute, Context *context, Node *node){
    ObjectNode *res = execute(context, &node->childs[0]);
    return newObjectNode(NTYPE_INT, ((ObjectList *)res->value)->length);
}

ObjectNode *op_Assert
    (ExecuteHandler execute, Context *context, Node *node){
    bool res = (execute(context, &node->childs[0])->value == 0);
    const char *assertion_name = node->childs_length > 1 ? node->childs[1].name : "";
    if(res)
        printf("FAILED");
    else
        printf("PASSED");
    printf("\t\tAssertion \"%s\" ", assertion_name);
    printf("\n");
    return newObjectNode(NTYPE_NONE, 0);
}

void addOp(Context *context, char *token, OpHandler handler){
    context_set(context, token, (void *) newObjectNode(NTYPE_BUILTIN_FUNC, handler));
}

void fillOpTable(Context *context){
    addOp(context, "+", &op_Plus);
    addOp(context, "-", &op_Minus);
    addOp(context, "*", &op_Mul);
    addOp(context, "/", &op_Div);
    addOp(context, "==", &op_Eq);
    addOp(context, "help", &op_Help);
    addOp(context, "define", &op_Define);
    addOp(context, "lambda", &op_Fn);
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
    addOp(context, "assert", &op_Assert);
}
