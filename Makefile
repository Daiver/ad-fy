CC = gcc
CXX = g++
#CC = clang

##bin/main: bin/hashtable_test
bin/ad-fy: build/hashtable.o build/common.o build/lexer.o build/parser.o build/stack.o build/context.o build/builtins.o build/executor.o build/extloader.o build/bmp.o
	$(CC) --std=gnu99 main.c build/common.o build/lexer.o build/parser.o build/executor.o build/builtins.o build/hashtable.o build/context.o build/stack.o build/extloader.o build/bmp.o -o bin/ad-fy -w  -rdynamic -ldl

build/executor.o: build/context.o build/parser.o
	$(CC) --std=gnu99 executor.c -o build/executor.o -c -w

build/builtins.o: build/context.o build/parser.o
	$(CC) --std=gnu99 builtins.c -o build/builtins.o -c -w

build/parser.o: build/lexer.o build/common.o
	$(CC) --std=gnu99 parser.c -o build/parser.o -c -w

build/lexer.o: build/common.o
	$(CC) --std=gnu99 lexer.c -o build/lexer.o -c -w

build/context.o: build/stack.o build/hashtable.o build/common.o
	$(CC) --std=gnu99 context.c -o build/context.o -c -w

build/common.o:
	$(CC) --std=gnu99 common.c -c -o build/common.o -w

build/stack.o:
	$(CC) --std=gnu99 stack.c -o build/stack.o -c -w

build/hashtable.o:
	$(CC) --std=gnu99 hashtable.c -o build/hashtable.o -c -w

build/extloader.o: lib/libext1.so.1.0 lib/libcv.so.1.0 lib/libextbmp.so.1.0
	$(CC) --std=gnu99 extloader.c -o build/extloader.o -c -w


lib/libext1.so.1.0: build/ext1.o
	$(CC) --std=gnu99 -shared -Wl,-soname,libext1.so.1 -o lib/libext1.so.1.0 build/ext1.o -w 

build/ext1.o: 
	$(CC) --std=gnu99 -Wall -fPIC -c ext1.c -o build/ext1.o -w

lib/libcv.so.1.0: build/extcv.o
	$(CC) --std=gnu99 -shared -Wl,-soname,libcv.so.1 -o lib/libcv.so.1.0 build/extcv.o -w `pkg-config opencv --cflags --libs` 

build/extcv.o: 
	$(CC) --std=gnu99 -Wall -fPIC -c extcv.c -o build/extcv.o -w `pkg-config opencv --cflags --libs` 

lib/libextbmp.so.1.0: build/extbmp.o
	$(CC) --std=gnu99 -shared -Wl,-soname,libextbmp.so.1 -o lib/libextbmp.so.1.0 build/extbmp.o -w 

build/extbmp.o: build/bmp.o 
	$(CC) --std=gnu99 -Wall -fPIC -c extbmp.c -o build/extbmp.o -w

build/bmp.o:
	$(CC) --std=gnu99 bmp.c -o build/bmp.o -c -w


clean:
	rm bin/ad-fy
	rm build/*.o

#bin/hashtable_test:
#	$(CC) --std=gnu99 hashtable_test.c -o bin/hashtable_test
