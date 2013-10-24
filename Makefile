CC = gcc
#CC = clang

bin/main: bin/hashtable_test
	$(CC) --std=c99 main.c common.c lexer.c parser.c -o bin/ad-fy -w #-Wno-pointer-to-int-cast

bin/hashtable_test:
	$(CC) --std=c99 hashtable_test.c -o bin/hashtable_test
