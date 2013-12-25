#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "context.h"
#include "eapi.h"

ObjectNode *newObjectNode(unsigned char type, void *value);
ObjectList *newObjectList(int length, ObjectNode *items);
ObjectNode *execute(Context *context, Node *node);

#endif
