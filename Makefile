_CXXRT=/usr/local/lib/gcc$(GCCVER)
_BOOST=..

IBOOST?=$(_BOOST)
CXX=g++$(GCCVER)
CXXFLAGS=$(CXXSTD) $(CXXOPTFLAGS) $(CXXWFLAGS) -I$(IBOOST)
LDFLAGS=-Wl,-L $(LCXXRT) -Wl,-rpath $(RCXXRT)

CXXSTD=-std=c++0x
CXXOPTFLAGS=-O1
CXXWFLAGS=-Wall -Wextra -Werror -Wfatal-errors
LCXXRT=$(_CXXRT)
RCXXRT=$(_CXXRT)
LDLIBS=

GCCVER=44

all: iniphile

clean:
	rm -f iniphile *.o

check: iniphile
	./iniphile < lf.ini

iniphile.cpp: input.hpp output.hpp manip.hpp metagram.hpp

.PHONY: check clean
