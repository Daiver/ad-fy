bin/prog: bin/libext1.so.1.0 bin/libext2.so.1.0 build/hi.o
	gcc -rdynamic -o bin/prog prog.c build/hi.o -ldl -std=c99

bin/libext2.so.1.0: build/ext2.o
	gcc -shared -Wl,-soname,libext2.so.1 -o bin/libext2.so.1.0   build/ext2.o

bin/libext1.so.1.0: build/ext1.o 
	gcc -shared -Wl,-soname,libext1.so.1 -o bin/libext1.so.1.0   build/ext1.o 

build/ext1.o: build/hi.o
	gcc -Wall -fPIC -c ext1.c -o build/ext1.o build/hi.o

build/ext2.o:
	gcc -Wall -fPIC -c ext2.c -o build/ext2.o

build/hi.o:
	gcc -o build/hi.o hi.c -c

clean:
	rm build/* bin/*
