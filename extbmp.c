#include <stdlib.h>
#include <stdio.h>
#include "eapi.h"
#include "builtins.h"
#include "bmp.h"


void getExtContent(Operation **ops, int *len){
    *len = 1;
    *ops = malloc(sizeof(Operation ) * *len);
    (*ops)[0].name = "loadbmp";
    (*ops)[0].handler = "op_LoadBmp";
}

ObjectNode *op_LoadBmp
    (ExecuteHandler execute, Context *context, Node *node){
    void *res = NULL;
//    ObjectNode *objpath = execute(context, &node->childs[0]);
    FILE *f = fopen("a.bmp", "r");
    
    BITMAP bmp;
    if(readBitmap(&bmp, "pics/black8x8.bmp"))
      return newObjectNode(NTYPE_NONE, NULL);
    
    printInfo(&bmp);
    freeBitmap(&bmp);
    return newObjectNode(NTYPE_LIST, res);
}
