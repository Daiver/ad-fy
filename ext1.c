#include <stdlib.j>
#include "eapi.h"


void getExtContent(Operation **ops, int *len){
    *len = 1;
    *ops = malloc(sizeof(Operation *) * l);
    (*ops)[0].name = "xor";
    (*ops)[0].handler = "op_Xor";
}

ObjectNode *op_Xor
    (ExecuteHandler execute, Context *context, Node *node){

    int res = 0;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res ^ (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}
