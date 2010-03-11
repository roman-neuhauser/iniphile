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
CXX_c=-c
CXX_o=-o
AR_rc=-rc
LD=$(CXX)
LD_o=-o
LDFLAGS=-Wl,-L $(UTFLIB) -Wl,-rpath $(UTFRUN) \
	-Wl,-L $(CXXRTLIB) -Wl,-rpath $(CXXRTRUN)
LDFLAGS_SO=-shared -Wl,--soname=$(SONAME)
MKDIR_P=mkdir -p

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


COMPILE=$(CXX) $(CXXFLAGS) $(CXX_c) $(CXX_o)

LIBINIPHILE_PC=libiniphile.pc
CANONICAL=libiniphile.so
SONAME=$(CANONICAL).$(VERSION.major)
LIBOBJECTS=input.o output.o ast.o
OBJECTS=initest-shared.o initest-static.o $(LIBOBJECTS)

PUBLIC_HEADERS=ast.hpp astfwd.hpp declspec.hpp input.hpp metagram.hpp output.hpp

ARTIFACTS=$(OBJECTS) initest-static$(dot_exe) initest-shared$(dot_exe) \
	  $(SONAME) *.a *.pc

all: initest $(LIBINIPHILE_PC)

clean:
	$(RM_F) $(ARTIFACTS)

check: initest
	LD_LIBRARY_PATH=. ./initest-static$(dot_exe)
	LD_LIBRARY_PATH=. ./initest-shared$(dot_exe)

install: all
	$(MKDIR_P) $(DESTDIR)$(LIBDIR)
	$(INSTALL.stripped) libiniphile.a $(DESTDIR)$(LIBDIR)/libiniphile.a
	$(INSTALL.stripped) $(SONAME) $(DESTDIR)$(LIBDIR)/$(SONAME)
	cd $(DESTDIR)$(LIBDIR) \
		&& $(RM_F) $(CANONICAL) \
		&& $(LN_S) $(SONAME) $(CANONICAL)
	$(MKDIR_P) $(DESTDIR)$(INCDIR)
	for f in $(PUBLIC_HEADERS); do \
		$(INSTALL) $$f $(DESTDIR)$(INCDIR)/$$f; \
	done
	$(INSTALL) libiniphile.pc $(PKGCONFIGDIR)/libiniphile.pc

initest: initest-static$(dot_exe) initest-shared$(dot_exe)

libiniphile.pc: libiniphile.pc.in
	trap "$(RM_F) libiniphile.pc.$$$$" EXIT; \
	sed -e 's#@@PREFIX@@#$(PREFIX)#' \
	    -e 's#@@VERSION@@#$(VERSION.string)#' \
	    < libiniphile.pc.in \
	    > libiniphile.pc.$$$$; \
	mv libiniphile.pc.$$$$ libiniphile.pc

$(SONAME): $(LIBOBJECTS)
	$(LD) $(LDFLAGS) $(LDFLAGS_SO) $(LD_o)$(SONAME) $(LIBOBJECTS)

libiniphile.a: $(LIBOBJECTS)
	$(AR) $(ARFLAGS) $(AR_rc) libiniphile.a $(LIBOBJECTS)

initest-static$(dot_exe): initest-static.o libiniphile.a
	$(LD) $(LDFLAGS) $(LD_o)initest-static$(dot_exe) initest-static.o libiniphile.a $(LDLIBS)

initest-shared$(dot_exe): initest-shared.o
	$(LD) $(LDFLAGS) $(LD_o)initest-shared$(dot_exe) initest-shared.o -liniphile $(LDLIBS)

initest-static.o: initest.cpp
	$(COMPILE)$@ initest.cpp

initest-shared.o: initest.cpp
	$(COMPILE)$@ initest.cpp

.cpp.o:
	$(COMPILE)$@ $<

initest-shared$(dot_exe): $(SONAME)
initest-static.o: metagram.hpp input.hpp output.hpp ast.hpp
initest-shared.o: metagram.hpp input.hpp output.hpp ast.hpp
input.o: metagram.hpp input.hpp
output.o: metagram.hpp output.hpp ast.hpp
ast.o: metagram.hpp ast.hpp

.PHONY: all check clean initest
