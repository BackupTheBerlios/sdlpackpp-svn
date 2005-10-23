CC=$(CXX)
CXXFLAGS=-Wall -ansi -pedantic

all: hexstream.o hexdump

hexstream.o: hexstream.cc
hexstream.o: hexstream.h

hexdump: hexdump.o hexstream.o

hexdump.o: hexstream.h

clean:
	rm -f *.o hexdump

new: clean all
