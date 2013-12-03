#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "eapi.h"
#include "builtins.h"
#include "common.h"
#include "extloader.h"

bool load(void **lib_handle, int ext_index, const char *name,  Context *context){
  lib_handle[ext_index] = dlopen(name, RTLD_LAZY);
  if (!lib_handle[ext_index]) {
    fprintf(stderr, "%s\n", dlerror());
    return false;
  }

  ExtContentGetter fn = dlsym(lib_handle[ext_index], "getExtContent");
  char *error = dlerror();
  if (error != NULL){
    fprintf(stderr, "%s\n", error);
    return false;
  }

  Operation *ops;
  int len;
  fn(&ops, &len);
  for(int i = 0; i < len; ++i){
    OpHandler handler = dlsym(lib_handle[ext_index], ops[i].handler);
    printf("Loaded extension %s\n", ops[i].name);
    addOp(context, ops[i].name, handler);
  }
  return true;
}

int loadExtensions(char *location, Context *context){
   const char *ext_names[1] = {
        "/home/cyou/Projects/Other/C/ad-fy/lib/libext1.so.1.0"
   };
   int ext_num = 1;
   void **lib_handle = malloc(sizeof(void *) * ext_num);

   hashtable_t exthandlers;

   for (int i = 0; i < ext_num; ++i)
     load(lib_handle, i, ext_names[i], context);
   return ext_num;
}

int closeExtensions(void **lib_handle, int ext_num){
   //It does not remove extensions from context!
   for(int i = 0; i < ext_num; ++i)
     dlclose(lib_handle[i]);
   free(lib_handle);
}
