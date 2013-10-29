#include <stdio.h>
#include <dlfcn.h>
#include "eapi.h"

int main(int argc, char **argv) 
{
   void *lib_handle;
   int x;
   char *error;
   ExtContentGetter fn;

   lib_handle = dlopen("/home/daiver/coding/ad-fy/extensionwork/bin/libext1.so.1.0", RTLD_LAZY);
   if (!lib_handle) 
   {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
   }

   fn = dlsym(lib_handle, "getExtContent");
   if ((error = dlerror()) != NULL)  
   {
        fprintf(stderr, "%s\n", error);
        return 1;
   }

   const char **names;
   int len;
   fn(&names, &len);
   printf("len %d\n", len);
   for(int i = 0; i < len; i++){
        printf("[%d] %s\n", i, names[i]);
   }

   dlclose(lib_handle);
   return 0;
}
