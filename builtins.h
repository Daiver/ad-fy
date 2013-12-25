#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "executor.h"
#include "eapi.h"
#include "context.h"
#include "parser.h"

typedef ObjectNode*(*ExecuteHandler)(Context *, Node *);
typedef ObjectNode*(*OpHandler)(ExecuteHandler, Context *, Node *);
ObjectNode *newObjectNode(unsigned char type, void *value);
ObjectList *newObjectList(int length, ObjectNode *items);
ObjectNode *listFromString(const char *s);
ObjectNode *newException(const char *msg);
void fillOpTable(Context *operators);
void addOp(Context *context, char *token, OpHandler handler);

#endif
