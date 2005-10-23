CC=$(CXX)
CPPFLAGS=-I.. $(shell sdl-config --cflags)
LIB=-lm

.PHONY: ../libpack++.a ../genpack

all: test

../libpack++.a ../genpack:
	$(MAKE) -C ..

test: test.cc libt1.a ../libpack++.a ../../hexstream/hexstream.o

libt1.a: t1.o
	ar crv $@ $?

t1.o: t1.cc
t1.o: t1.h ../pack++.h

t1.cc t1.h: t1.pack
	../genpack t1.pack
t1.cc t1.h: ../genpack

#../genpack: ../genpack.cc ../Makefile
#	$(MAKE) -C .. genpack

clean:
	rm -f *.o *.a test

new: clean all
