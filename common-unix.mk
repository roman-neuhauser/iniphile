# vim: ts=8 noet ft=make

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
ARFLAGS=
AR_rc=-rc
LD=$(CXX)
LD_o=-o
LDFLAGS=-Wl,-L $(UTFLIB) -Wl,-rpath $(UTFRUN) \
	-Wl,-L $(CXXRTLIB) -Wl,-rpath $(CXXRTRUN)
LDFLAGS.shared=-Wl,-L$$PWD -Wl,-rpath $$PWD
LDFLAGS.static=
LDFLAGS_SO=-shared -Wl,--soname=$(SONAME)
MKDIR_P=mkdir -p

CXXSTD=-std=c++98 -pedantic
CXXOPTFLAGS=-g -O1
CXXWFLAGS=-Wall -Wextra -Werror -Wfatal-errors -Wno-long-long
CXXRTLIB=$(_CXXRT)
CXXRTRUN=$(_CXXRT)
LDLIBS=-lboost_unit_test_framework
LDLIBS_SHARED=-liniphile

GCCVER?=44

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

check: initest
	LD_LIBRARY_PATH=. ./initest-static$(dot_exe)
	LD_LIBRARY_PATH=. ./initest-shared$(dot_exe)

install: all
	$(MKDIR_P) $(DESTDIR)$(LIBDIR)
	$(INSTALL_PROGRAM) libiniphile.a $(DESTDIR)$(LIBDIR)/libiniphile.a
	$(INSTALL_PROGRAM) $(SONAME) $(DESTDIR)$(LIBDIR)/$(SONAME)
	cd $(DESTDIR)$(LIBDIR) \
		&& $(RM_F) $(CANONICAL) \
		&& $(LN_S) $(SONAME) $(CANONICAL)
	$(MKDIR_P) $(DESTDIR)$(INCDIR)
	for f in $(PUBLIC_HEADERS); do \
		$(INSTALL) $$f $(DESTDIR)$(INCDIR)/$$f; \
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

