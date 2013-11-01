#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"
#include "stack.h"
#include "context.h"
#include "common.h"

hashtable_t *findScope(Context *context, char *key){
    hashtable_t *scope = NULL;
    Stack temp = stack_new();
    void *res = NULL;
    do{
        scope = (hashtable_t *) stack_pop(&context->scopes);
        if(scope){
            stack_push(&temp, scope);
            res = ht_get(scope, key);
        }
    }while(scope && !res);
    while(!stack_isEmpty(&temp))
        stack_push(&context->scopes, stack_pop(&temp));
    return scope;
}

void *context_get(Context *context, char *key){
    if(!context)
        return NULL;
    hashtable_t *scope = findScope(context, key);
    return scope ? ht_get(scope, key) : NULL;
}

void context_set(Context *context, char *key, void *value, bool localy){
    if(!context || stack_isEmpty(&context->scopes))
        return;
    hashtable_t *scope = NULL;
    hashtable_t *localScope = stack_pick(&context->scopes);
    if(localy){
        scope = localScope;
    }else{
        hashtable_t *tmp = stack_pop(&context->scopes);
        scope = findScope(context, key);
        stack_push(&context->scopes, tmp);
        if(!scope)
            scope = localScope;
    }
    ht_set(scope, key, (char *)value);
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

/*int main(){
    char *val = NULL;
    Context *context = context_new();

    context_enterScope(context);
    context_set(context, "a", "valA", true);
    context_set(context, "b", "tmpValB", true);
    context_set(context, "b", "valB", true);
    val = (char*) context_get(context, "b");
    printf("%s\n", val ? val : "Not found");
    val = (char*) context_get(context, "a");
    printf("%s\n", val ? val : "Not found");
    printf("\n");

    context_enterScope(context);
    context_set(context, "c", "valC", true);
    val = (char*) context_get(context, "a");
    printf("%s\n", val ? val : "Not found");
    context_set(context, "a", "localValA", true);
    val = (char*) context_get(context, "c");
    printf("%s\n", val ? val : "Not found");
    val = (char*) context_get(context, "a");
    printf("%s\n", val ? val : "Not found");
    context_set(context, "a", "newValA", false);
    val = (char*) context_get(context, "a");
    printf("%s\n", val ? val : "Not found");
    printf("\n");

    context_leaveScope(context);
    val = (char*) context_get(context, "a");
    printf("%s\n", val ? val : "Not found");
    val = (char*) context_get(context, "b");
    printf("%s\n", val ? val : "Not found");
    val = (char*) context_get(context, "c");
    printf("%s\n", val ? val : "Not found");


    context_leaveScope(context);
    context_remove(context);
    return 0;
}*/
