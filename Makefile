bin/main: bin/hashtable_test
	gcc --std=c99 main.c -o bin/main -w #-Wno-pointer-to-int-cast
#	clang -o bin/main main.c

bin/hashtable_test:
	gcc --std=c99 hashtable_test.c -o bin/hashtable_test
