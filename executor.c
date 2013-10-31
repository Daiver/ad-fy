#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"
#include "builtins.h"
#include "context.h"

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

ObjectNode *execute(Context *context, Node *node){
    if(isDigit(node->name))
        return newObjectNode(NTYPE_INT, (void *) atoi(node->name));
    ObjectNode *obj = context_get(context, node->name);
    if(obj == NULL){
        printf("Execution error> Object does not exists: [%s]\n", node->name);
        return newObjectNode(NTYPE_NONE, 0);
    } 
    switch(obj->type){
        case NTYPE_BUILTIN_FUNC : {
            OpHandler func = obj->value;
            return func(context, node);
        }
        case NTYPE_NODE : {
            return execute(context, obj->value);
        }
        case NTYPE_FUNC : {
            FunctionObj *func = (FunctionObj *) obj->value;
            ObjectNode **arguments = malloc(sizeof(ObjectNode *) * node->childs_length);
            for(int i = 0; i < node->childs_length; i++)
                arguments[i] = execute(context, &node->childs[i]);
            //Unfair contexts save
            //void **backup = malloc(sizeof(void *) * func->args_length);
            for(int i = 0; i < func->args_length; i++){
                //void *tmp = context_get(context, func->args[i]);
                //if(tmp != NULL){
                //    backup[i] = tmp;
                //    context_del(context, func->args[i]);
                //}
                //else
                //    backup[i] = NULL;
                context_set(context, func->args[i], arguments[i]);
            }
            int i;
            for(i = 0; i < func->node_length - 1; i++)
                free(execute(context, func->nodes[i]));
            ObjectNode *res = execute(context, func->nodes[i]);
            //Unfair context resotore
            //for(i = 0; i < func->args_length; i++){
            //    context_del(context, func->args[i]);
            //    if(backup[i] != NULL){
            //        context_set(context, func->args[i], backup[i]);
            //    }
            //}
            //free(backup); 
            free(arguments);
            return res;
        }
        default : {
            if(obj->type > 100)
                return obj;
        }
    }
    printf("Execution error> Unknown type: [%d]\n", obj->type);
    return newObjectNode(NTYPE_NONE, 0);
}
