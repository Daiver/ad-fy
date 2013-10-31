#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "context.h"

#define NTYPE_NONE 0
#define NTYPE_BUILTIN_FUNC 1
#define NTYPE_NODE 2
#define NTYPE_FUNC 3
#define NTYPE_INT 101
#define NTYPE_BOOL 105
#define NTYPE_LIST 110

struct TObjectNode{
    unsigned char type;
    void *value;
};
typedef struct TObjectNode ObjectNode;

struct TFunctionObj{
    int args_length;
    const char *prefix;
    char **args;
    int node_length;
    Node **nodes;
};
typedef struct TFunctionObj FunctionObj;

struct TObjectList{
    int length;
    ObjectNode *items;
};
typedef struct TObjectList ObjectList;

ObjectNode *newObjectNode(unsigned char type, void *value);
ObjectList *newObjectList(int length, ObjectNode *items);
ObjectNode *execute(Context *context, Node *node);

#endif
