_CXXRT=/usr/local/lib/gcc$(GCCVER)
_BOOST=..

IBOOST?=$(_BOOST)
CXX=g++$(GCCVER)
CXX=env CXX=g++$(GCCVER) gfilt
CXXFLAGS=$(CXXSTD) $(CXXOPTFLAGS) $(CXXWFLAGS) -I$(IBOOST)
LDFLAGS=-Wl,-L $(LCXXRT) -Wl,-rpath $(RCXXRT)

CXXSTD=-std=c++98 -pedantic
CXXOPTFLAGS=-g -O1
CXXWFLAGS=-Wall -Wextra -Werror -Wfatal-errors -Wno-long-long
LCXXRT=$(_CXXRT)
RCXXRT=$(_CXXRT)
LDLIBS=

GCCVER?=44

all: iniphiletest iniphile.so

clean:
	rm -f iniphiletest *.so *.a *.o

check: iniphiletest
	./iniphiletest < lf.ini

iniphile.so: iniphile.a
	$(CXX) -shared -o iniphile.so iniphile.a

iniphile.a: output.o ast.o input.o
	$(AR) -rc iniphile.a output.o ast.o input.o

iniphiletest: iniphiletest.o iniphile.a
	$(CXX) $(LDFLAGS) -o iniphiletest iniphiletest.o iniphile.a $(LDLIBS)

iniphiletest.cpp: input.hpp output.hpp manip.hpp ast.hpp metagram.hpp
input.cpp: input.hpp metagram.hpp
output.cpp: output.hpp metagram.hpp ast.hpp
ast.cpp: ast.hpp metagram.hpp manip.hpp

.PHONY: check clean
