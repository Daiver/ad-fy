#include <stdlib.h>
#include "eapi.h"
#include "builtins.h"


void getExtContent(Operation **ops, int *len){
    *len = 2;
    *ops = malloc(sizeof(Operation ) * *len);
    (*ops)[0].name = "xor";
    (*ops)[0].handler = "op_Xor";
    (*ops)[1].name = "readfile";
    (*ops)[1].handler = "op_ReadFile";
}

char *readFAsL(char *input_file_name){
    char *file_contents;
    long input_file_size;
    FILE *input_file = fopen(input_file_name, "rb");
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = malloc((input_file_size) * (sizeof(char)));
    fread(file_contents, sizeof(char), input_file_size, input_file);
    fclose(input_file);
    //file_contents[input_file_size] = 0;
    return file_contents;
}

ObjectNode *op_ReadFile
    (ExecuteHandler execute, Context *context, Node *node){
    ObjectNode *fname_obj = execute(context, &node->childs[0]);
    ObjectList *li = fname_obj->value;
    char *fname = malloc(sizeof(char) * (li->length + 1));
    for(int i = 0; i < li->length; i++){
        fname[i] = (char)li->items[i].value;
    }
    char *s = readFAsL(fname);
    return newObjectNode(NTYPE_STRING, listFromString(s));
}

ObjectNode *op_Xor
    (ExecuteHandler execute, Context *context, Node *node){

    int res = 0;
    for(int i = 0; i < node->childs_length; i++){
        ObjectNode *tmp = execute(context, &node->childs[i]);
        res ^= (int)tmp->value;
    }
    return newObjectNode(NTYPE_INT, res);
}


