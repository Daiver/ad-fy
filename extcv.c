#include <stdlib.h>
#include "eapi.h"
#include "builtins.h"


void getExtContent(Operation **ops, int *len){
    *len = 1;
    *ops = malloc(sizeof(Operation *) * *len);
    (*ops)[0].name = "xor1";
    (*ops)[0].handler = "op_Xor1";
}

ObjectNode *op_Xor1
    (ExecuteHandler execute, Context *context, Node *node){

    int res = 0;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res ^= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}

