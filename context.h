#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"
#include "stack.h"
#include "common.h"

typedef struct TContext Context;
struct TContext{
    Stack scopes;
};

void *context_get(Context *context, char *key);
void context_set(Context *context, char *key, void *value, bool localy);
void context_enterScope(Context *context);
void context_leaveScope(Context *context);
Context *context_new();

#endif
