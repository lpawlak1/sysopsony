CC = gcc -Wall -std=c11 -ggdb


all: static shared clean

test:
	bash tests/tests.sh

static:
	$(CC) -c library.c
	ar rcs liblibrary.a library.o

shared:
	$(CC) -c -fPIC library.c
	$(CC) -shared -fPIC -o library.so library.o

clean:
	rm -f *.o *.a *.so esasa temp main
