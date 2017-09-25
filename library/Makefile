CFLAGS := -m64 -O2 -g -D_GNU_SOURCE -D_REENTRANT -W -I/usr/local/include
LDFLAGS := -m64 -lm

all: npheap.c
	$(CC) $(CFLAGS) -Wall -fPIC -c npheap.c
	$(CC) $(CFLAGS) -shared -Wl,-soname,libnpheap.so.1 -o libnpheap.so.1.0 npheap.o

install: libnpheap.so.1.0
	cp libnpheap.so.1.0 /usr/lib/libnpheap.so.1
	ln -fs /usr/lib/libnpheap.so.1 /usr/lib/libnpheap.so
	cp npheap.h  /usr/local/include


clean:
	rm -f *.so *.o


.PHONY: all clean
