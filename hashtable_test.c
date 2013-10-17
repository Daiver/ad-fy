#include <stdio.h>
#include "hashtable.h"

void simpleTest(){
    hashtable_t *hashtable = ht_create( 65536 );
 
    ht_set( hashtable, "key1", "inky" );
    ht_set( hashtable, "key2", "pinky" );
    ht_set( hashtable, "key3", "blinky" );
    ht_set( hashtable, "key4", "floyd" );
 
    printf( "%s\n", ht_get( hashtable, "key1" ) );
    printf( "%s\n", ht_get( hashtable, "key2" ) );
    printf( "%s\n", ht_get( hashtable, "key3" ) );
    printf( "%s\n", ht_get( hashtable, "key4" ) );
    ht_del(hashtable, "key4");
    char* tmp =  ht_get( hashtable, "key4" );
    if(tmp == NULL)
        printf("NO KEY\n");

}

int main(int argc, char **argv){
    printf("test started\n");
    simpleTest();
    return 0;
}
