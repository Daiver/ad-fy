#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "stack.h"

StackItem *newItem(StackItem *next, void *value){
    StackItem *item = (StackItem *) malloc(sizeof(StackItem));
    item->next = next;
    item->value = value;
    return item;
}

void stack_push(Stack *stack, void *value){
    StackItem *item = newItem(stack->head, value);
    stack->head = item;
}

void freeItem(StackItem *item){
    if(item){
        free(item);
    }
}

void *stack_pop(Stack *stack){
    if(stack && stack->head){
        StackItem *item = stack->head;
        stack->head = stack->head->next;
        void *value = item->value;
        freeItem(item);
        return value;
    }
    return NULL;
}

void *stack_pick(Stack *stack){
    if(stack && stack->head){
        return stack->head->value;
    }
    return NULL;
}

bool stack_isEmpty(Stack *stack){
  return stack == NULL || stack->head == NULL;
}

Stack stack_new(){
    Stack stack = {NULL};
    return stack;
}

void *stack_find(Stack *stack, Condition cond){
    if(!(stack && stack->head))
      return NULL;
   
    StackItem *item = stack->head;
    while(item){
        if(cond(item->value))
            return item->value;
        item = item->next;     
    }
    return NULL;
}

/*
bool check(void *value){
   return ((char *) value)[0] == 'o';
}

int main(){
    Stack stack = stack_new();

    stack_push(&stack, "a");
    stack_push(&stack, "c");
    stack_push(&stack, "b");
    
    char *v = (char *) stack_find(&stack, &check);
    if(!v) printf("Not found\n");
    else printf("%s\n", v);
    return 0;
}
*/
