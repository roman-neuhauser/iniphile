PREFIX?=/usr/local
LIBDIR?=$(PREFIX)/lib
INCDIR?=$(PREFIX)/include/iniphile
PKGCONFIGDIR?=$(LIBDIR)/pkgconfig

_CXXRT?=/usr/local/lib/gcc$(GCCVER)
_BOOST?=..

SPIRIT?=$(_BOOST)/include
UTFINC?=$(_BOOST)/include
UTFLIB?=$(_BOOST)/lib
UTFRUN?=$(UTFLIB)
CXXFLAGS=$(CXXSTD) $(CXXOPTFLAGS) $(CXXWFLAGS) -I$(SPIRIT) -I$(UTFINC)
CXX=env CXX=g++$(GCCVER) gfilt
CXX=g++$(GCCVER)
LD=$(CXX)
LDFLAGS=-Wl,-L $(UTFLIB) -Wl,-rpath $(UTFRUN) \
	-Wl,-L $(CXXRTLIB) -Wl,-rpath $(CXXRTRUN)

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
VERSION.minor=1
VERSION.string=$(VERSION.major).$(VERSION.minor)

CANONICAL=libiniphile.so
SONAME=$(CANONICAL).$(VERSION.major)
WL.SONAME=-Wl,--soname=$(SONAME)

PUBLIC_HEADERS=ast.hpp astfwd.hpp input.hpp metagram.hpp output.hpp

all: initest libiniphile.pc

clean:
	rm -f initest-static initest-shared *.so.[0-9] *.a *.o *.pc

check: initest
	LD_LIBRARY_PATH=. ./initest-static
	LD_LIBRARY_PATH=. ./initest-shared

install: all
	mkdir -p $(DESTDIR)$(LIBDIR)
	$(INSTALL.stripped) libiniphile.a $(DESTDIR)$(LIBDIR)/libiniphile.a
	$(INSTALL.stripped) $(SONAME) $(DESTDIR)$(LIBDIR)/$(SONAME)
	cd $(DESTDIR)$(LIBDIR) \
		&& $(RM_F) $(CANONICAL) \
		&& $(LN_S) $(SONAME) $(CANONICAL)
	mkdir -p $(DESTDIR)$(INCDIR)
	for f in $(PUBLIC_HEADERS); do \
		$(INSTALL) $$f $(DESTDIR)$(INCDIR)/$$f; \
	done
	$(INSTALL) libiniphile.pc $(PKGCONFIGDIR)/libiniphile.pc

initest: initest-static initest-shared

libiniphile.pc: libiniphile.pc.in
	trap "$(RM_F) libiniphile.pc.$$$$" EXIT; \
	sed -e 's#@@PREFIX@@#$(PREFIX)#' \
	    -e 's#@@VERSION@@#$(VERSION.string)#' \
	    < libiniphile.pc.in \
	    > libiniphile.pc.$$$$; \
	mv libiniphile.pc.$$$$ libiniphile.pc

$(SONAME): libiniphile.a
	$(LD) -shared $(WL.SONAME) -o $(SONAME) -Wl,--whole-archive libiniphile.a

libiniphile.a: output.o ast.o input.o
	$(AR) -rc libiniphile.a output.o ast.o input.o

initest-static: initest.o libiniphile.a
	$(LD) $(LDFLAGS) -o initest-static initest.o libiniphile.a $(LDLIBS)

initest-shared: initest.o $(SONAME)
	$(LD) $(LDFLAGS) -o initest-shared initest.o -liniphile $(LDLIBS)

initest.o: metagram.hpp input.hpp output.hpp ast.hpp
input.o: metagram.hpp input.hpp
output.o: metagram.hpp output.hpp ast.hpp
ast.o: metagram.hpp ast.hpp

.PHONY: all check clean initest
