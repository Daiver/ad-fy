#include <stdio.h>
#include <dlfcn.h>
#include "eapi.h"
#include "hi.h"

int main(int argc, char **argv) 
{
   int x;
   hu();
   char *error;
   ExtContentGetter fn;
   const char *ext_names[2] = { 
        "/home/daiver/coding/ad-fy/extensionwork/bin/libext1.so.1.0",
        "/home/daiver/coding/ad-fy/extensionwork/bin/libext2.so.1.0"
   };
   int ext_num = 2;
   void **lib_handle;
   lib_handle = malloc(sizeof(void *) * ext_num);

   for (int ext_index = 0; ext_index < ext_num; ext_index++){
       lib_handle[ext_index] = dlopen(ext_names[ext_index] , RTLD_LAZY);
       if (!lib_handle) 
       {
            fprintf(stderr, "%s\n", dlerror());
            return 1;
       }

       fn = dlsym(lib_handle[ext_index], "getExtContent");
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
            func = dlsym(lib_handle[ext_index], names[i + 1]);
            printf(">%d\n", func(10));
       }
   }
   for(int i = 0; i < ext_num; i++){
       dlclose(lib_handle[i]);
   }
   free(lib_handle);
   return 0;
}
