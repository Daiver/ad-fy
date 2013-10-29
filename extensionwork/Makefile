prog: libext1.so.1.0
	gcc -rdynamic -o prog prog.c -ldl -std=c99

libext1.so.1.0: ext1.o
	gcc -shared -Wl,-soname,libext1.so.1 -o libext1.so.1.0   ext1.o

ext1.o:
	gcc -Wall -fPIC -c ext1.c

clean:
	rm *.o prog *.so*
