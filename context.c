/*void *context_get(Context *context, char *key);
void context_set(Context *context, char *key, void *value);
void context_enterScope(Context *context);
void context_leaveScope(Context *context);
Context *context_new();*/


#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"
#include "stack.h"

struct TContext{
    Stack scopes;
};
typedef struct TContext Context;

//void *context_get(Context *context, char *key);
void context_set(Context *context, char *key, void *value){
     if(!context || stack_isEmpty(&context->scopes))
         return;
     hashtable_t *scope = stack_pick(&context->scopes);
     ht_set(scope, key, value);
}

void context_enterScope(Context *context){
    if(!context)
        return;
     hashtable_t *scope = ht_create(128);
     stack_push(&context->scopes, scope);
}


void context_leaveScope(Context *context){
    if(context && !stack_isEmpty(&context->scopes)){
       hashtable_t *scope = stack_pop(&context->scopes);
       free(scope);
    }
}

Context *context_new(){
    Context *context = (Context *) malloc(sizeof(Context));
    context->scopes = stack_new();
    return context;
}


void context_remove(Context *context){
    if(context){
        free(context);
    }
}


int main(){
    Context *context = context_new();
    context_enterScope(context);
    context_set(context, "a", "valA");
    context_set(context, "b", "valB");
    context_leaveScope(context);
    context_remove(context);
    return 0;
}
