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
LDFLAGS.shared=-Wl,-L$$PWD -Wl,-rpath $$PWD
LDFLAGS.static=

CXXSTD=-std=c++98 -pedantic
CXXOPTFLAGS=-g -O1
CXXWFLAGS=-Wall -Wextra -Werror -Wfatal-errors -Wno-long-long
CXXRTLIB=$(_CXXRT)
CXXRTRUN=$(_CXXRT)
LDLIBS=-lboost_unit_test_framework

GCCVER?=44

LN_S?=ln -s
RM_F?=rm -f

VERSION.major=0

SONAME=libiniphile.so.$(VERSION.major)
WL.SONAME=-Wl,--soname=$(SONAME)

all: initest

clean:
	$(RM_F) initest-static initest-shared $(SONAME) *.so *.a *.o

check: initest
	./initest-static
	./initest-shared

initest: initest-static initest-shared

$(SONAME): libiniphile.a
	$(CXX) -shared $(WL.SONAME) -o $(SONAME) -Wl,--whole-archive libiniphile.a

libiniphile.so: $(SONAME)
	$(RM_F) libiniphile.so
	$(LN_S) $(SONAME) libiniphile.so

libiniphile.a: output.o ast.o input.o
	$(AR) -rc libiniphile.a output.o ast.o input.o

initest-static: initest.o libiniphile.a
	$(CXX) $(LDFLAGS) $(LDFLAGS.static) -o initest-static initest.o libiniphile.a $(LDLIBS)

initest-shared: initest.o libiniphile.so $(SONAME)
	$(CXX) $(LDFLAGS) $(LDFLAGS.shared) -o initest-shared initest.o -liniphile $(LDLIBS)

initest.o: metagram.hpp input.hpp output.hpp ast.hpp
input.o: metagram.hpp input.hpp
output.o: metagram.hpp output.hpp manip.hpp ast.hpp
ast.o: metagram.hpp ast.hpp manip.hpp

.PHONY: all check clean initest
