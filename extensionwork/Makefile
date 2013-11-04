bin/prog: bin/libext1.so.1.0 bin/libext2.so.1.0
	gcc -rdynamic -o bin/prog prog.c -ldl -std=c99

bin/libext2.so.1.0: build/ext2.o
	gcc -shared -Wl,-soname,libext2.so.1 -o bin/libext2.so.1.0   build/ext2.o

bin/libext1.so.1.0: build/ext1.o
	gcc -shared -Wl,-soname,libext1.so.1 -o bin/libext1.so.1.0   build/ext1.o

build/ext1.o:
	gcc -Wall -fPIC -c ext1.c -o build/ext1.o

build/ext2.o:
	gcc -Wall -fPIC -c ext2.c -o build/ext2.o

clean:
	rm build/* bin/*
