#ifndef __EAPI_H__
#define __EAPI_H__

#include <stdlib.h>
#include "context.h"

#define NTYPE_NONE 0
#define NTYPE_BUILTIN_FUNC 1
#define NTYPE_NODE 2
#define NTYPE_FUNC 3
#define NTYPE_INT 101
#define NTYPE_DOUBLE 102
#define NTYPE_BOOL 105
#define NTYPE_LIST 110

struct TNode{
    const char *name;
    unsigned int pos_in_line;
    unsigned int line;
    unsigned int childs_length;
    struct TNode *childs;
};
typedef struct TNode Node;

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
    Context *context;
};
typedef struct TFunctionObj FunctionObj;

struct TObjectList{
    int length;
    ObjectNode *items;
};
typedef struct TObjectList ObjectList;

struct TOperation{
  char *name;
  char *handler;
};
typedef struct TOperation Operation;

typedef void (*ExtContentGetter)(Operation **ops, int *len);

#endif
