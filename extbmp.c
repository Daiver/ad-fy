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

ObjectNode *mkList(BITMAP *bmp){
    ObjectList *res = (ObjectList *)malloc(sizeof(ObjectList));
    res->items = (ObjectNode *)malloc(sizeof(ObjectNode) * bmp->size);
    res->length = bmp->size;
    for(int i = 0; i < bmp->size; i++)
        res->items[i] = *newObjectNode(NTYPE_INT, (void *) bmp->data[i]);
    return newObjectNode(NTYPE_LIST, res); 
}

ObjectNode *op_LoadBmp
    (ExecuteHandler execute, Context *context, Node *node){
    void *res = NULL;
//    ObjectNode *objpath = execute(context, &node->childs[0]);
    BITMAP bmp;
    if(readBitmap(&bmp, "pics/black8x8.bmp"))
      return newObjectNode(NTYPE_NONE, NULL);
    res = (void *) mkList(&bmp);
    freeBitmap(&bmp);
//    return newObjectNode(NTYPE_LIST, res);
    return res;
}
