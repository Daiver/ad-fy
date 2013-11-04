CC = gcc
#CC = clang

##bin/main: bin/hashtable_test
bin/ad-fy: build/hashtable.o build/common.o build/lexer.o build/parser.o
	$(CC) --std=c99 main.c build/common.o build/lexer.o build/parser.o executor.c builtins.c build/hashtable.o context.c stack.c -o bin/ad-fy -w 

build/parser.o:
	$(CC) --std=c99 parser.c -o build/parser.o build/lexer.o build/common.o -c

build/lexer.o: build/common.o
	$(CC) --std=c99 lexer.c -o build/lexer.o -c

build/common.o:
	$(CC) --std=c99 common.c -c -o build/common.o

build/hashtable.o:
	$(CC) --std=c99 hashtable.c -o build/hashtable.o -c

clean:
	rm bin/ad-fy
	rm build/*.o

#bin/hashtable_test:
#	$(CC) --std=c99 hashtable_test.c -o bin/hashtable_test
