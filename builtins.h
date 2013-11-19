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
/*ObjectNode *op_Plus(Context *context, Node *node);
ObjectNode *op_Mul(Context *context, Node *node);
ObjectNode *op_Minus(Context *context, Node *node);
ObjectNode *op_Eq(Context *context, Node *node);
ObjectNode *op_Div(Context *context, Node *node);
ObjectNode *op_Help(Context *context, Node *node);
ObjectNode *op_Define(Context *context, Node *node);
ObjectNode *op_Alias(Context *context, Node *node);
ObjectNode *op_Fn(Context *context, Node *node);
ObjectNode *op_DefFn(Context *context, Node *node);
ObjectNode *op_Quote(Context *context, Node *node);
ObjectNode *op_If(Context *context, Node *node);
ObjectNode *op_Import(Context *context, Node *node);
ObjectNode *op_Comment(Context *context, Node *node);
ObjectNode *op_Print(Context *context, Node *node);
ObjectNode *op_List(Context *context, Node *node);
ObjectNode *op_Elem(Context *context, Node *node);
ObjectNode *op_Slice(Context *context, Node *node);
ObjectNode *op_Cons(Context *context, Node *node);
ObjectNode *op_Length(Context *context, Node *node);*/
void fillOpTable(Context *operators);
void addOp(Context *context, char *token, OpHandler handler);

#endif
