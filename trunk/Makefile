CC=$(CXX)
CPPFLAGS=-Wall -I.. $(shell sdl-config --cflags)

all: genpack libpack++.a

genpack: genpack.cc

libpack++.a: pack++.o
	ar crv $@ $?

pack++.o: pack++.cc
pack++.o: pack++.h

clean:
	rm -f *.o *.a genpack

new: clean all
