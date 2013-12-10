#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
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

//"/home/cyou/Projects/Other/C/ad-fy/lib/libext1.so.1.0"
int loadExtensions(char *location, Context *context){
  DIR *dir = opendir(location);
  struct dirent *ent;
  if (dir == NULL)
    return 0;
  int ext_num = 0;
  int ext_length = 5;
  char **ext_names = (char **) malloc(ext_length * sizeof(char *));
  while ((ent = readdir (dir)) != NULL)
    if (ent->d_type == DT_REG) {
       if(ext_num >= ext_length)
         *ext_names = (char **) realloc(ext_names, ext_length*=2);
       ext_names[ext_num]  = ent->d_name;
       ++ext_num;
    }
  closedir(dir);
  void **lib_handle = (void **) malloc(sizeof(void **) * ext_num);
  hashtable_t exthandlers;
  for (int i = 0; i < ext_num; ++i)
    load(lib_handle, i, ext_names[i], context);
  free(lib_handle);
  free(ext_names);
  return ext_num;
}

int closeExtensions(void **lib_handle, int ext_num){
   //It does not remove extensions from context!
   for(int i = 0; i < ext_num; ++i)
     dlclose(lib_handle[i]);
   free(lib_handle);
}
