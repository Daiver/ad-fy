bin/prog: bin/libext1.so.1.0
	gcc -rdynamic -o bin/prog prog.c -ldl -std=c99

bin/libext1.so.1.0: build/ext1.o
	gcc -shared -Wl,-soname,libext1.so.1 -o bin/libext1.so.1.0   build/ext1.o

build/ext1.o:
	gcc -Wall -fPIC -c ext1.c -o build/ext1.o

clean:
	rm build/* bin/*
