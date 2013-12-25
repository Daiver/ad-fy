#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include <stdio.h>
#include "common.h"

typedef bool(*Condition)(void *);
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
void *stack_pick(Stack *stack);
bool stack_isEmpty(Stack *stack);
void *stack_find(Stack *stack, Condition cond);

#endif
