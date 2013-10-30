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
    ObjectNode *res = newObjectNode(NTYPE_NONE, 0);
    //printf("[%s]\n", node->name);
    if(isDigit(node->name)){
        res->type = NTYPE_INT;
        res->value = (void *)atoi(node->name);
    }
    else{
        ObjectNode *obj = ht_get(hashtable, node->name);
        if(obj == NULL){
            printf("EXECTE ERROR obj [%s] does not exists\n", node->name);
            return res;
        }
        if(obj->type == NTYPE_BUILTIN_FUNC){
            ObjectNode *(*fp)(hashtable_t *hashtable, Node *node) = obj->value;
            res = fp(hashtable, node);
        }else if(obj->type == NTYPE_NODE){
            return execute(hashtable, obj->value);
        }else if(obj->type == NTYPE_FUNC){// MAKE ITBETTER. PLEASE ='(
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
