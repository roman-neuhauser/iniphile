CXX=g++$(GCCVER)
CXXFLAGS=-std=c++0x -O1 -Wall -Werror -Wfatal-errors -I..
RUNTIME=/usr/local/lib/gcc$(GCCVER)
LDFLAGS=-L$(RUNTIME) -rpath $(RUNTIME)
LDLIBS=

GCCVER=44

all: iniphile

clean:
	rm -f iniphile *.o

check: iniphile
	./iniphile < lf.ini

iniphile.cpp: input.hpp output.hpp manip.hpp metagram.hpp

.PHONY: check clean
