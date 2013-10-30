#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include <stdio.h>
#include "common.h"

typedef struct TStackItem StackItem;
typedef struct TStack Stack;
struct TStackItem{
    StackItem *next;
    void *value;
};
struct TStack{
    StackItem *head;
};
Stack stack_new();
void stack_push(Stack *stack, void *value);
void *stack_pop(Stack *stack);
bool stack_isEmpty(Stack *stack);

#endif
