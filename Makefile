CC = gcc
#CC = clang

##bin/main: bin/hashtable_test
bin/ad-fy: build/hashtable.o build/common.o build/lexer.o build/parser.o build/stack.o build/context.o build/builtins.o build/executor.o
	$(CC) --std=c99 main.c build/common.o build/lexer.o build/parser.o build/executor.o build/builtins.o build/hashtable.o build/context.o build/stack.o -o bin/ad-fy -w 

build/executor.o: build/context.o build/parser.o
	$(CC) --std=c99 executor.c build/context.o build/parser.o -o build/executor.o -c

build/builtins.o: build/context.o build/parser.o
	$(CC) --std=c99 builtins.c build/parser.o build/context.o -o build/builtins.o -c

build/parser.o: build/lexer.o build/common.o
	$(CC) --std=c99 parser.c -o build/parser.o build/lexer.o build/common.o -c

build/lexer.o: build/common.o
	$(CC) --std=c99 lexer.c -o build/lexer.o -c

build/context.o: build/stack.o build/hashtable.o build/common.o
	$(CC) --std=c99 context.c build/stack.o build/hashtable.o build/common.o -o build/context.o -c

build/common.o:
	$(CC) --std=c99 common.c -c -o build/common.o

build/stack.o:
	$(CC) --std=c99 stack.c -o build/stack.o build/common.o -c

build/hashtable.o:
	$(CC) --std=c99 hashtable.c -o build/hashtable.o -c

clean:
	rm bin/ad-fy
	rm build/*.o

#bin/hashtable_test:
#	$(CC) --std=c99 hashtable_test.c -o bin/hashtable_test
