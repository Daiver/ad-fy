CC = gcc
#CC = clang

##bin/main: bin/hashtable_test
bin/ad-fy: build/hashtable.o build/common.o
	$(CC) --std=c99 main.c build/common.o lexer.c parser.c executor.c builtins.c build/hashtable.o context.c stack.c -o bin/ad-fy -w #-Wno-pointer-to-int-cast

build/common.o:
	$(CC) --std=c99 common.c -c -o build/common.o

build/hashtable.o:
	$(CC) --std=c99 hashtable.c -o build/hashtable.o -c

#bin/hashtable_test:
#	$(CC) --std=c99 hashtable_test.c -o bin/hashtable_test
