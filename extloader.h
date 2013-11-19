#include <stdlib.h>
#include "builtins.h"
#include "context.h"

int loadExtensions(char *location, Context *context);
int closeExtensions(void **lib_handle, int ext_num);
