_CXXRT?=/usr/local/lib/gcc$(GCCVER)
_BOOST?=..

SPIRIT?=$(_BOOST)/include
UTFINC?=$(_BOOST)/include
UTFLIB?=$(_BOOST)/lib
UTFRUN?=$(UTFLIB)
CXX?=env CXX=g++$(GCCVER) gfilt
CXX?=g++$(GCCVER)
CXXFLAGS=$(CXXSTD) $(CXXOPTFLAGS) $(CXXWFLAGS) -I$(SPIRIT) -I$(UTFINC)
LDFLAGS=-Wl,-L $(UTFLIB) -Wl,-rpath $(UTFRUN) \
	-Wl,-L $(CXXRTLIB) -Wl,-rpath $(CXXRTRUN)

CXXSTD=-std=c++98 -pedantic
CXXOPTFLAGS=-g -O1
CXXWFLAGS=-Wall -Wextra -Werror -Wfatal-errors -Wno-long-long
CXXRTLIB=$(_CXXRT)
CXXRTRUN=$(_CXXRT)
LDLIBS=-lboost_unit_test_framework

GCCVER?=44

all: initest iniphile.so

clean:
	rm -f initest *.so *.a *.o

check: initest
	./initest < lf.ini

iniphile.so: iniphile.a
	$(CXX) -shared -o iniphile.so iniphile.a

iniphile.a: output.o ast.o input.o
	$(AR) -rc iniphile.a output.o ast.o input.o

initest: initest.o iniphile.a
	$(CXX) $(LDFLAGS) -o initest initest.o iniphile.a $(LDLIBS)

initest.cpp: input.hpp output.hpp manip.hpp ast.hpp metagram.hpp
input.cpp: input.hpp metagram.hpp
output.cpp: output.hpp metagram.hpp ast.hpp
ast.cpp: ast.hpp metagram.hpp manip.hpp

.PHONY: check clean
