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

char * listToString(ObjectNode *fname_obj){
   ObjectList *li = fname_obj->value;
    char *fname = malloc(sizeof(char) * (li->length + 1));
    for(int i = 0; i < li->length; i++){
        fname[i] = (char)li->items[i].value;
    }
    fname[li->length] = '\0';
    return fname;
}

ObjectNode *op_LoadBmp
    (ExecuteHandler execute, Context *context, Node *node){
    void *res = NULL;
    ObjectNode *objpath = execute(context, &node->childs[0]);

    char *path = listToString(objpath);
    BITMAP bmp;
    if(readBitmap(&bmp, path))
      return newObjectNode(NTYPE_NONE, NULL);
    res = (void *) mkList(&bmp);
    freeBitmap(&bmp);
    return res;
}
