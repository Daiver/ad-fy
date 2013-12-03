CC = gcc
#CC = clang

##bin/main: bin/hashtable_test
bin/ad-fy: build/hashtable.o build/common.o build/lexer.o build/parser.o build/stack.o build/context.o build/builtins.o build/executor.o build/extloader.o
	$(CC) --std=c99 main.c build/common.o build/lexer.o build/parser.o build/executor.o build/builtins.o build/hashtable.o build/context.o build/stack.o build/extloader.o -o bin/ad-fy -w  -rdynamic -ldl

build/executor.o: build/context.o build/parser.o
	$(CC) --std=c99 executor.c -o build/executor.o -c -w

build/builtins.o: build/context.o build/parser.o
	$(CC) --std=c99 builtins.c -o build/builtins.o -c -w

build/parser.o: build/lexer.o build/common.o
	$(CC) --std=c99 parser.c -o build/parser.o -c -w

build/lexer.o: build/common.o
	$(CC) --std=c99 lexer.c -o build/lexer.o -c -w

build/context.o: build/stack.o build/hashtable.o build/common.o
	$(CC) --std=c99 context.c -o build/context.o -c -w

build/common.o:
	$(CC) --std=c99 common.c -c -o build/common.o -w

build/stack.o:
	$(CC) --std=c99 stack.c -o build/stack.o -c -w

build/hashtable.o:
	$(CC) --std=c99 hashtable.c -o build/hashtable.o -c -w

build/extloader.o: lib/libext1.so.1.0
	$(CC) --std=c99 extloader.c -o build/extloader.o -c -w

lib/libext1.so.1.0: build/ext1.o
	$(CC) --std=c99 -shared -Wl,-soname,libext1.so.1 -o lib/libext1.so.1.0 build/ext1.o -w

build/ext1.o: 
	$(CC) --std=c99 -Wall -fPIC -c ext1.c -o build/ext1.o -w


clean:
	rm bin/ad-fy
	rm build/*.o

#bin/hashtable_test:
#	$(CC) --std=c99 hashtable_test.c -o bin/hashtable_test
