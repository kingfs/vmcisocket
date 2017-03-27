CC=gcc
CFLAGS=-fPIC -pedantic -Werror -Wall -std=gnu90
SOFLAGS=-shared 

.PHONY: clean all debug

vmci/libvmci.so: vmcilib.o
	$(CC) $(CFLAGS) $(SOFLAGS) -o $@ $<
	@rm $^

vmcilib.o: vmcilib.c vmcilib.h vmci_sockets.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.so *.pyc

