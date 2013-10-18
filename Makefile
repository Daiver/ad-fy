#CC = gcc
CC = clang

bin/main: bin/hashtable_test
	$(CC) --std=c99 main.c -o bin/main -w #-Wno-pointer-to-int-cast

bin/hashtable_test:
	$(CC) --std=c99 hashtable_test.c -o bin/hashtable_test
