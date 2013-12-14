#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "context.h"
#include "logging.h"
#include "extloader.h"

#define EXT_LOCATION "lib/"

void import(Context *context, const char *fname){
    LOG("import", "begin");
    const char *source = readFileAsLine(fname);
    LOG("import", "file read");
    TokenStream ts;
    LOG("import", "filling token stream");
    //printf(">[%s] [%s]\n",fname, source);
    fillTokenStream(&ts, source);
    LOG("import", "token stream filled");
    while(!isEndOfStream(&ts)){ 
        LOG("import", "calling parse");
        Node head = parse(&ts, 0);
        LOG("import", "returned from parse");
        LOG("import", "calling execute");
        ObjectNode *node = execute(context, &head);
        LOG("import", "returned from execute");
    }
}

void testExecute(const char *source, bool verbose){
    LOG("testExecuteSecond", "begin");
    Context *globalContext = context_new();
    context_enterScope(globalContext); 
    LOG("testExecuteSecond", "filling op table");
    fillOpTable(globalContext);
    LOG("testExecuteSecond", "loading extensions");
    int ext_num = loadExtensions(EXT_LOCATION, globalContext);
    printf("Loaded %d extensions\n", ext_num);
    LOG("testExecuteSecond", "op table filled");
    import(globalContext, "stl.x");
    TokenStream ts;
    LOG("testExecuteSecond", "starting fill token stream");
    fillTokenStream(&ts, source); 
    LOG("testExecuteSecond", "token stream filled");
    while(!isEndOfStream(&ts)){
        LOG("testExecuteSecond", "calling parse");
        Node head = parse(&ts, 0);
        LOG("testExecuteSecond", "returning form parse");
        LOG("TestExecuteSecond", "calling execute");
        if(verbose)
            printTree(head, 0);
        LOG("testExecuteSecond", "calling execute");
        ObjectNode *node = execute(globalContext, &head);
        if(verbose){
            printf("res>");
            printObjectNode(node);
            printf("\n");
        }
    }
//    closeExtensions(lib_handle, ext_num);
    context_leaveScope(globalContext);
}


void REPL(bool verbose){
    Context *globalContext = context_new();
    context_enterScope(globalContext); 
    fillOpTable(globalContext);
    int ext_num = loadExtensions(EXT_LOCATION, globalContext);
    printf("Loaded %d extensions\n", ext_num);
    import(globalContext, "stl.x");
    int n_bytes = 100;
    char *source = malloc(n_bytes + 1);
    for(int i = 0;i < n_bytes + 1;i++) source[i] = 0;
    while (1){
        getline(&source, &n_bytes, stdin);
        //printf("%s\n", source);
        TokenStream ts;
        fillTokenStream(&ts, source); 
        while(!isEndOfStream(&ts)){
            Node head = parse(&ts, 0);
            if(verbose)
                printTree(head, 0);
            ObjectNode *node = execute(globalContext, &head);
            printf("res>");
            printObjectNode(node);
            printf("\n");
        }
    }
//    closeExtensions(lib_handle, ext_num);
    context_leaveScope(globalContext);
}

int main(int argc, char **argv){
    if(argc > 1){
        bool verbose = false;
        bool repl = false;
        bool from_arg = false;
        for(int i = 1; i < argc; i++){
            if(strcmp(argv[i], "-i") == 0)
                repl = true;
            if(strcmp(argv[i], "-v") == 0)
                verbose = true;
            if(strcmp(argv[i], "-s") == 0)
                from_arg = true;
        }
        if(repl){
            REPL(verbose);
            return 0;
        }
        printf("Reading from file [%s]...\n", argv[argc - 1]);
        const char *src = from_arg ? argv[argc - 1] : readFileAsLine(argv[argc - 1]);
        printf("{-\n%s\n-}\n", src);
        testExecute(src, verbose);
    }
    else{
        printf("USAGE ./main [-v|-i|-s] source_file");
    }
    return 0;
}
