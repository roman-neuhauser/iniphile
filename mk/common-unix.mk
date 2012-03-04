# vim: ts=8 noet ft=make

PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin
LIBDIR?=$(PREFIX)/lib
INCDIR?=$(PREFIX)/include
PKGCONFIGDIR?=$(LIBDIR)/pkgconfig
MANDIR?=$(PREFIX)/man
MAN1DIR?=$(MANDIR)/man1

RST2HTML?=rst2html.py

_CXXRT?=/usr/lib
_BOOST?=$(PREFIX)

SPIRIT?=$(_BOOST)/include
UTFINC?=$(_BOOST)/include
UTFLIB?=$(_BOOST)/lib
UTFRUN?=$(UTFLIB)
CXXFLAGS=$(CXXSTD) $(CXXOPTFLAGS) $(CXXWFLAGS) -I$(SPIRIT) -I$(UTFINC)
CXX=env CXX=g++$(GCCVER) gfilt
CXX=g++$(GCCVER)
CXX_c=-c
CXX_o=-o
ARFLAGS=-rc
LD=$(CXX)
LD_o=-o
LDFLAGS=-Wl,-L $(UTFLIB) -Wl,-rpath $(UTFRUN) \
	-Wl,-L $(CXXRTLIB) -Wl,-rpath $(CXXRTRUN)
LDFLAGS_shared=-Wl,-L$$PWD -Wl,-rpath $$PWD
LDFLAGS_static=
LDFLAGS_SO=-shared -Wl,--soname=$(SONAME)
MKDIR_P=mkdir -p

CXXSTD?=-std=c++98 -pedantic
CXXOPTFLAGS=-g -O1
CXXWFLAGS=-Wall -Wextra -Wfatal-errors -Wno-long-long
CXXRTLIB=$(_CXXRT)
CXXRTRUN=$(_CXXRT)
LDLIBS=-lboost_unit_test_framework
LDLIBS_SHARED=-liniphile

GCCVER?=

LN_S?=ln -s
RM_F?=rm -f
INSTALL_PROGRAM?=$(INSTALL) -s

DLL_LINKAGE=

LIBINIPHILE_PC=libiniphile.pc
SONAME=$(CANONICAL).$(VERSION_major)
IMPORT_LIB=$(CANONICAL)

EMBED_MANIFEST= \
	:

dot_exe=

all: .all

check: initest check-solink
	LD_LIBRARY_PATH=. ./initest-static$(dot_exe)
	LD_LIBRARY_PATH=. ./initest-shared$(dot_exe)

ifndef HAVE_CHECK_SOLINK
check-solink: initest-shared
	@if ! ldd initest-shared | grep -Fq /$(SONAME); then \
	  echo initest-shared does not seem to use $(SONAME); \
	  false; \
	fi
endif

install: all
	$(MKDIR_P) $(DESTDIR)$(BINDIR)
	$(MKDIR_P) $(DESTDIR)$(LIBDIR)
	$(MKDIR_P) $(DESTDIR)$(MAN1DIR)
	$(INSTALL_PROGRAM) libiniphile.a $(DESTDIR)$(LIBDIR)/libiniphile.a
	$(INSTALL_PROGRAM) $(SONAME) $(DESTDIR)$(LIBDIR)/$(SONAME)
	cd $(DESTDIR)$(LIBDIR) \
		&& $(RM_F) $(CANONICAL) \
		&& $(LN_S) $(SONAME) $(CANONICAL)
	$(INSTALL_PROGRAM) iniphile $(DESTDIR)$(BINDIR)/iniphile
	$(INSTALL) iniphile.1 $(DESTDIR)$(MAN1DIR)/iniphile.1
	$(MKDIR_P) $(DESTDIR)$(INCDIR)/iniphile
	for f in $(PUBLIC_HEADERS); do \
		$(INSTALL) $$f $(DESTDIR)$(INCDIR)/iniphile/$$f; \
	done
	$(INSTALL) libiniphile.pc $(PKGCONFIGDIR)/libiniphile.pc

libiniphile.pc: libiniphile.pc.in
	trap "$(RM_F) libiniphile.pc.$$$$" EXIT; \
	sed -e 's#@@PREFIX@@#$(PREFIX)#' \
	    -e 's#@@VERSION@@#$(VERSIONSTRING)#' \
	    < libiniphile.pc.in \
	    > libiniphile.pc.$$$$; \
	mv libiniphile.pc.$$$$ libiniphile.pc

libiniphile.so: $(SONAME)
	$(RM_F) libiniphile.so
	$(LN_S) $(SONAME) libiniphile.so

