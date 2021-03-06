#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"
#include "stack.h"
#include "context.h"

void *context_get(Context *context, char *key){
    if(!context)
        return NULL;
    hashtable_t *scope = NULL;
    void *res = NULL;  
    Stack temp = stack_new();
    do{
      scope = (hashtable_t *) stack_pop(&context->scopes);
      if(scope){
         stack_push(&temp, scope);
         res = ht_get(scope, key);
      }
    }while(scope && !res);

    while(!stack_isEmpty(&temp))
        stack_push(&context->scopes, stack_pop(&temp));

    return res ? res : NULL;
}

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
//       free(scope);
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

/*
int main(){
    char *val = NULL;
    Context *context = context_new();

    context_enterScope(context);
    context_set(context, "a", "valA");
    context_set(context, "b", "valB");
    val = (char*) context_get(context, "b");
    printf("%s\n", val);

    context_enterScope(context);
    context_set(context, "c", "valC");
    val = (char*) context_get(context, "a");
    printf("%s\n", val);
    context_set(context, "a", "localValA");
    val = (char*) context_get(context, "c");
    printf("%s\n", val);
    val = (char*) context_get(context, "a");
    printf("%s\n", val);

    context_leaveScope(context);
    val = (char*) context_get(context, "a");
    printf("%s\n", val);
    val = (char*) context_get(context, "c");
    printf("%s\n", val ? val : "Not found");


    context_leaveScope(context);
    context_remove(context);
    return 0;
}*/
