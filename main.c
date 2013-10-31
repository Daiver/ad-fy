#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "context.h"

void import(hashtable_t *hashtable, const char *fname){
    const char *source = readFileAsLine(fname);
    TokenStream ts;
    fillTokenStream(&ts, source);
    while(!isEndOfStream(&ts)){
        Node head = parse(&ts, 0);
        ObjectNode *node = execute(hashtable, &head);
    }
}

void testExecuteSecond(const char *source){
    TokenStream ts;
    fillTokenStream(&ts, source);
    Context *globalContext = context_new();
    fillOpTable(globalContext);
    import(hashtable, "stl.x");
    while(!isEndOfStream(&ts)){
        Node head = parse(&ts, 0);
        printTree(head, 0);
        ObjectNode *node = execute(hashtable, &head);
        printf("res>");
        printObjectNode(node);
        printf("\n");
    }
}

int main(int argc, char **argv){
    if(argc > 1){
        printf("Reading from file [%s]...\n", argv[1]);
        const char *src = readFileAsLine(argv[1]);
        printf("{-\n%s\n-}\n", src);
        testExecuteSecond(src);
    }
    else{
        printf("USAGE ./main source_file");
    }
    return 0;
}
