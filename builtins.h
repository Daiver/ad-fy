#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"
#include "hashtable.h"
#include "parser.h"

typedef ObjectNode*(*OpHandler)(hashtable_t, Node *);
ObjectNode *op_Plus(hashtable_t *hashtable, Node *node);
ObjectNode *op_Mul(hashtable_t *hashtable, Node *node);
ObjectNode *op_Minus(hashtable_t *hashtable, Node *node);
ObjectNode *op_Eq(hashtable_t *hashtable, Node *node);
ObjectNode *op_Div(hashtable_t *hashtable, Node *node);
ObjectNode *op_Help(hashtable_t *hashtable, Node *node);
ObjectNode *op_Define(hashtable_t *hashtable, Node *node);
ObjectNode *op_Alias(hashtable_t *hashtable, Node *node);
ObjectNode *op_Fn(hashtable_t *hashtable, Node *node);
ObjectNode *op_DefFn(hashtable_t *hashtable, Node *node);
ObjectNode *op_Quote(hashtable_t *hashtable, Node *node);
ObjectNode *op_If(hashtable_t *hashtable, Node *node);
ObjectNode *op_Import(hashtable_t *hashtable, Node *node);
ObjectNode *op_Comment(hashtable_t *hashtable, Node *node);
ObjectNode *op_Print(hashtable_t *hashtable, Node *node);
ObjectNode *op_List(hashtable_t *hashtable, Node *node);
ObjectNode *op_Elem(hashtable_t *hashtable, Node *node);
ObjectNode *op_Slice(hashtable_t *hashtable, Node *node);
ObjectNode *op_Cons(hashtable_t *hashtable, Node *node);
ObjectNode *op_Length(hashtable_t *hashtable, Node *node);
void fillOpTable(hashtable_t *operators);

#endif
