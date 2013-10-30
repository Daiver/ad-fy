#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"
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

ObjectNode *execute(hashtable_t *hashtable, Node *node){
    if(isDigit(node->name))
        return newObjectNode(NTYPE_INT, (void *) atoi(node->name));
    ObjectNode *obj = ht_get(hashtable, node->name);
    if(obj == NULL){
        printf("Execution error> Object does not exists: [%s]\n", node->name);
        return newObjectNode(NTYPE_NONE, 0);
    } 
    switch(obj->type){
        case NTYPE_BUILTIN_FUNC : {
            OpHandler func = obj->value;
            return func(hashtable, node);
        }
        case NTYPE_NODE : {
            return execute(hashtable, obj->value);
        }
        case NTYPE_FUNC : {
            FunctionObj *func = (FunctionObj *) obj->value;
            ObjectNode **arguments = malloc(sizeof(ObjectNode *) * node->childs_length);
            for(int i = 0; i < node->childs_length; i++)
                arguments[i] = execute(hashtable, &node->childs[i]);
            //Unfair contexts save
            void **backup = malloc(sizeof(void *) * func->args_length);
            for(int i = 0; i < func->args_length; i++){
                void *tmp = ht_get(hashtable, func->args[i]);
                if(tmp != NULL){
                    backup[i] = tmp;
                    ht_del(hashtable, func->args[i]);
                }
                else
                    backup[i] = NULL;
                ht_set(hashtable, func->args[i], arguments[i]);
            }
            int i;
            for(i = 0; i < func->node_length - 1; i++)
                free(execute(hashtable, func->nodes[i]));
            ObjectNode *res = execute(hashtable, func->nodes[i]);
            //Unfair context resotore
            for(i = 0; i < func->args_length; i++){
                ht_del(hashtable, func->args[i]);
                if(backup[i] != NULL){
                    ht_set(hashtable, func->args[i], backup[i]);
                }
            }
            free(backup); 
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
