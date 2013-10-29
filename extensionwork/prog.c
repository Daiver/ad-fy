#include <stdio.h>
#include <dlfcn.h>
#include "eapi.h"

int main(int argc, char **argv) 
{
   void *lib_handle;
   int x;
   char *error;
   ExtContentGetter fn;
   const char *ext_names[2] = { 
        "/home/daiver/coding/ad-fy/extensionwork/bin/libext1.so.1.0",
        "/home/daiver/coding/ad-fy/extensionwork/bin/libext2.so.1.0"
   };
   lib_handle = dlopen(ext_names[1] , RTLD_LAZY);
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
   for(int i = 0; i < len; i += 2){
        printf("[%d] %s - %s\n", i, names[i], names[i + 1]);
        ExtBuiltInFunction func;
        func = dlsym(lib_handle, names[i + 1]);
        printf(">%d\n", func(10));
   }

   dlclose(lib_handle);
   return 0;
}
