/*void *context_get(Context *context, char *key);
void context_set(Context *context, char *key, void *value);
void context_enterScope(Context *context);
void context_leaveScope(Context *context);
Context *context_new();*/


#include <stdlib.h>
#include <stdio.h>

struct TContext{
    Stack scopes;
}
typedef struct TContext Context;


void *context_get(Context *context, char *key);
void context_set(Context *context, char *key, void *value);
void context_enterScope(Context *context);
void context_leaveScope(Context *context);
Context *context_new();

int main(){


}
