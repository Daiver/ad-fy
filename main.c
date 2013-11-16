#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "context.h"
#include "logging.h"

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
        execute(context, &head);
        LOG("import", "returned from execute");
    }
}

void testExecute(const char *source, bool verbose){
    LOG("testExecuteSecond", "begin");
    Context *globalContext = context_new();
    context_enterScope(globalContext); 
    LOG("testExecuteSecond", "filling op table");
    fillOpTable(globalContext);
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
    context_leaveScope(globalContext);
}

int main(int argc, char **argv){
    if(argc > 1){
        bool verbose = false;
        for(int i = 1; i < argc; i++){
            if(strcmp(argv[i], "-v") == 0){
                verbose = true;
            }
        }
        printf("Reading from file [%s]...\n", argv[argc - 1]);
        const char *src = readFileAsLine(argv[argc - 1]);
        printf("{-\n%s\n-}\n", src);
        testExecute(src, verbose);
    }
    else{
        printf("USAGE ./main [-v] source_file");
    }
    return 0;
}
