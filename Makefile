PREFIX?=/usr/local
LIBDIR?=$(PREFIX)/lib
INCDIR?=$(PREFIX)/include/iniphile

_CXXRT?=/usr/local/lib/gcc$(GCCVER)
_BOOST?=..

SPIRIT?=$(_BOOST)/include
UTFINC?=$(_BOOST)/include
UTFLIB?=$(_BOOST)/lib
UTFRUN?=$(UTFLIB)
CXXFLAGS=$(CXXSTD) $(CXXOPTFLAGS) $(CXXWFLAGS) -I$(SPIRIT) -I$(UTFINC)
CXX=env CXX=g++$(GCCVER) gfilt
CXX=g++$(GCCVER)
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
INSTALL.stripped?=$(INSTALL) -s

VERSION.major=0

CANONICAL=libiniphile.so
SONAME=$(CANONICAL).$(VERSION.major)
WL.SONAME=-Wl,--soname=$(SONAME)

PUBLIC_HEADERS=ast.hpp astfwd.hpp input.hpp metagram.hpp output.hpp

all: initest

clean:
	rm -f initest-static initest-shared *.so *.a *.o

check: initest
	./initest-static
	./initest-shared

install: all
	mkdir -p $(DESTDIR)$(LIBDIR)
	$(INSTALL.stripped) libiniphile.a $(DESTDIR)$(LIBDIR)/libiniphile.a
	$(INSTALL.stripped) libiniphile.so $(DESTDIR)$(LIBDIR)/$(SONAME)
	cd $(DESTDIR)$(LIBDIR) \
		&& $(RM_F) $(CANONICAL) \
		&& $(LN_S) $(SONAME) $(CANONICAL)
	mkdir -p $(DESTDIR)$(INCDIR)
	for f in $(PUBLIC_HEADERS); do \
		$(INSTALL) $$f $(DESTDIR)$(INCDIR)/$$f; \
	done

initest: initest-static initest-shared

libiniphile.so: libiniphile.a
	$(CXX) -shared $(WL.SONAME) -o libiniphile.so -Wl,--whole-archive libiniphile.a

libiniphile.a: output.o ast.o input.o
	$(AR) -rc libiniphile.a output.o ast.o input.o

initest-static: initest.o libiniphile.a
	$(CXX) $(LDFLAGS) $(LDFLAGS.static) -o initest-static initest.o libiniphile.a $(LDLIBS)

initest-shared: initest.o libiniphile.so
	$(CXX) $(LDFLAGS) $(LDFLAGS.shared) -o initest-shared initest.o -liniphile $(LDLIBS)

initest.o: metagram.hpp input.hpp output.hpp ast.hpp
input.o: metagram.hpp input.hpp
output.o: metagram.hpp output.hpp manip.hpp ast.hpp
ast.o: metagram.hpp ast.hpp manip.hpp

.PHONY: all check clean initest
