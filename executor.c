#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"
#include "builtins.h"
#include "context.h"
#include "logging.h"

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
    LOG("execute", "begin");
    int numtype = checkIsNumber(node->name);
    if(numtype != NTYPE_NONE){
      LOG("execute", "Diggit found");
      void *num = NULL;
      switch(numtype){
        case NTYPE_INT : num = (void *) atoi(node->name); LOG("execute", "Int"); break;
        case NTYPE_DOUBLE : 
            num = (void *) strtod(node->name, NULL) ; LOG("execute", "Double"); break;
      }
 
      return newObjectNode(numtype, num);
    }


    LOG("execute", "getting object from context");
    ObjectNode *obj = context_get(context, node->name);
    LOG("execute", "object got");
    if(obj == NULL){
        if(strcmp(node->name, "") == 0 )
            printf("Execution error>Empty node\n");
        else
            printf("Execution error> Object does not exists: [%s]. At line %d %d \n", node->name, node->line, node->pos_in_line);
        return newObjectNode(NTYPE_NONE, 0);
    } 
    ObjectNode *result = NULL;
    LOG("execute", "type swtich start");
    switch(obj->type){
        case NTYPE_BUILTIN_FUNC : {
            LOG("execute", "builtin_func start");
            OpHandler func = obj->value;
            result = func(&execute, context, node);
            LOG("execute", "builtin_func end");
            break;
        }
        case NTYPE_NODE : {
            LOG("execute", "node start");
            result = execute(context, obj->value);
            LOG("execute", "node end");
            break;
        }
        case NTYPE_FUNC : {
            LOG("execute", "func start");
            FunctionObj *func = (FunctionObj *) obj->value;
            ObjectNode **arguments = malloc(sizeof(ObjectNode *) * node->childs_length);
            int i;

            LOG("execute", "entering scope");
            context_enterScope(func->context);
            for(i = 0; i < node->childs_length; i++)
                arguments[i] = execute(context, &node->childs[i]);
            for(i = 0; i < func->args_length; i++)
                context_set(func->context, func->args[i], arguments[i]);
            for(i = 0; i < func->node_length - 1; i++)
                free(execute(func->context, func->nodes[i]));
            result = execute(func->context, func->nodes[i]);
            free(arguments);
            LOG("execute", "func end");
            LOG("execute", "type swtich end");
            context_leaveScope(func->context);
            break;
        }
        default : {
            LOG("execute", "default start");
            if(obj->type > 100){
                LOG("execute", "type > 100");
                result = obj;
            }else{
	        LOG("execute", "type < 100");
                printf("Execution error> Unknown type: [%d] At line %d %d\n", obj->type, node->line, node->pos_in_line);
                return newObjectNode(NTYPE_NONE, 0);
            }
            LOG("execute", "default end");
        }
    }
    LOG("execute", "scope left");
    return result ? result : newObjectNode(NTYPE_NONE, 0);
}
