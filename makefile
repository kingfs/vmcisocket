###############################################################################
# Author	: Cory Levy
# Date 		: 27/03/2017
###############################################################################
CC=gcc
CFLAGS=-fPIC -pedantic -Werror -Wall -std=gnu90
SOFLAGS=-shared 
TARGET=vmci/libvmci.so
ARCH86=-m32
ARCH64=-m64

.PHONY: clean all debug x86 x64

# Build to the running machine architecture
$(TARGET): vmcilib.o
	$(CC) $(CFLAGS) $(SOFLAGS) -o $@ $<
	@rm $^

# Build to x86 architecture
x86: vmcilib.o
	$(CC) $(CFLAGS) $(SOFLAGS) $(ARCH86) -o $(TARGET) $<
	@rm $^

# Build to x64 architecture
x64: vmcilib.o
	$(CC) $(CFLAGS) $(SOFLAGS) $(ARCH64) -o $(TARGET) $<
	@rm $^

# Build object file
vmcilib.o: vmcilib.c vmcilib.h vmci_sockets.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.so *.pyc

