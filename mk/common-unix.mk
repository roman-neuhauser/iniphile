# vim: ts=8 noet ft=make

PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin
LIBDIR?=$(PREFIX)/lib
INCDIR?=$(PREFIX)/include
PKGCONFIGDIR?=$(LIBDIR)/pkgconfig
MANDIR?=$(PREFIX)/man
MAN1DIR?=$(MANDIR)/man1

GZIP?=gzip
RPMBUILD?=rpmbuild
RST2HTML?=rst2html.py

_CXXRT?=/usr/lib
_BOOST?=$(PREFIX)

SPIRIT?=$(_BOOST)/include
UTFINC?=$(_BOOST)/include
UTFLIB?=$(_BOOST)/lib
UTFRUN?=$(UTFLIB)
LINK_MODE?=shared
ifeq ($(LINK_MODE), shared)
UTFFLAGS=-DBOOST_TEST_DYN_LINK
UTF_LINKMODE=-Wl,-Bdynamic
else
UTF_LINKMODE=-Wl,-Bstatic
endif
CXXFLAGS=$(CXXSTD) -fPIC $(CXXOPTFLAGS) $(CXXWFLAGS) -I$(SPIRIT) -I$(UTFINC) -DBOOST_ALL_NO_LIB $(UTFFLAGS)
CXX=env CXX=g++$(GCCVER) gfilt
CXX=g++$(GCCVER)
CXX_c=-c
CXX_o=-o
ARFLAGS=-rc
LD=$(CXX)
LD_o=-o
LDFLAGS=-L$(UTFLIB) -Wl,-rpath $(UTFRUN) \
	-L$(CXXRTLIB) -Wl,-rpath $(CXXRTRUN)
LDFLAGS_shared=-L$$PWD -Wl,-rpath $$PWD
LDFLAGS_static=
LDFLAGS_SO=-shared -Wl,--soname=$(SONAME)
MKDIR_P=mkdir -p

CXXSTD?=-std=c++98 -pedantic
CXXOPTFLAGS=-g -O1
CXXWFLAGS=-Wall -Wextra -Wfatal-errors -Wno-long-long
CXXRTLIB=$(_CXXRT)
CXXRTRUN=$(_CXXRT)
LDLIBS=$(UTF_LINKMODE) -lboost_unit_test_framework
LDLIBS_SHARED=-Bdynamic -liniphile

GCCVER?=

LN_S?=ln -s
RM_F?=rm -f
INSTALL?=install
INSTALL_DATA?=$(INSTALL) -m 0644
INSTALL_LIBRARY?=$(INSTALL)
INSTALL_PROGRAM?=$(INSTALL) -s

DLL_LINKAGE=

INIPHILE_PC=iniphile.pc
INIPHILE_SPEC=iniphile.spec
SONAME=$(CANONICAL).$(VERSION_major)
IMPORT_LIB=$(CANONICAL)

EMBED_MANIFEST= \
	:

CWDREL=./

dot_exe=

all: .all iniphile.1.gz

check-1: iniphile
	SHELL=$(SHELL) $(SHELL) rnt/run-tests.sh tests $$PWD/iniphile

check-3: initest check-solink
	LD_LIBRARY_PATH=. PATH=$$PATH:$(UTFRUN) ./initest-static$(dot_exe)
	LD_LIBRARY_PATH=. PATH=$$PATH:$(UTFRUN) ./initest-shared$(dot_exe)

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
	$(INSTALL_DATA) libiniphile.a $(DESTDIR)$(LIBDIR)/libiniphile.a
	$(INSTALL_LIBRARY) $(SONAME) $(DESTDIR)$(LIBDIR)/$(SONAME)
	cd $(DESTDIR)$(LIBDIR) \
		&& $(RM_F) $(CANONICAL) \
		&& $(LN_S) $(SONAME) $(CANONICAL)
	$(INSTALL_PROGRAM) iniphile $(DESTDIR)$(BINDIR)/iniphile
	$(INSTALL_DATA) iniphile.1.gz $(DESTDIR)$(MAN1DIR)/iniphile.1.gz
	$(MKDIR_P) $(DESTDIR)$(INCDIR)/iniphile
	for f in $(PUBLIC_HEADERS); do \
		$(INSTALL_DATA) $$f $(DESTDIR)$(INCDIR)/iniphile/$$f; \
	done
	$(MKDIR_P) $(DESTDIR)$(PKGCONFIGDIR)
	$(INSTALL_DATA) $(INIPHILE_PC) $(DESTDIR)$(PKGCONFIGDIR)/$(INIPHILE_PC)

$(INIPHILE_PC): $(INIPHILE_PC).in
	trap "$(RM_F) $(INIPHILE_PC).$$$$" EXIT; \
	sed -e 's#@@PREFIX@@#$(PREFIX)#' \
	    -e 's#@@VERSION@@#$(VERSIONSTRING)#' \
	    < $(INIPHILE_PC).in \
	    > $(INIPHILE_PC).$$$$; \
	mv $(INIPHILE_PC).$$$$ $(INIPHILE_PC)

$(INIPHILE_SPEC): $(INIPHILE_SPEC).in
	trap "$(RM_F) $(INIPHILE_SPEC).$$$$" EXIT; \
	sed -e 's#__VERSION__#$(VERSIONSTRING)#' \
	    < $(INIPHILE_SPEC).in \
	    > $(INIPHILE_SPEC).$$$$; \
	mv $(INIPHILE_SPEC).$$$$ $(INIPHILE_SPEC)

%.1.gz: %.1
	$(GZIP) < $< > $@

rpm: iniphile-$(VERSION).rpm

iniphile-$(VERSION).rpm: $(INIPHILE_SPEC)
	$(RPMBUILD) -bb $(INIPHILE_SPEC)

libiniphile.so: $(SONAME)
	$(RM_F) libiniphile.so
	$(LN_S) $(SONAME) libiniphile.so

