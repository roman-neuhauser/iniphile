PREFIX=/usr/local
LIBDIR=$(PREFIX)\lib
INCDIR=$(PREFIX)\include\iniphile
BOOST_INCDIR=
BOOST_LIBDIR=
PATH=$(BOOST_LIBDIR);$(PATH)
CXXFLAGS=/nologo /EHsc /MD /I$(BOOST_INCDIR)
CXX=cl.exe
CXX_c=/c
CXX_o=/Fo
AR=lib.exe
ARFLAGS=/nologo
AR_rc=/OUT:
CP=copy
LD=link.exe
LD_L=/LIBPATH:
LD_o=/OUT:
LD_shared=/DLL
LDFLAGS=/nologo /INCREMENTAL:NO $(LD_L)$(BOOST_LIBDIR)
LDLIBS=boost_unit_test_framework.lib
MKDIR_P=mkdir
MT=mt.exe
MTFLAGS=/nologo
RM_F=del /f
INSTALL=copy

VERSION_major=0
VERSION_minor=1
VERSION_string=$(VERSION_major).$(VERSION_minor)

LIBINIPHILE_PC=
CANONICAL=libiniphile.so
SONAME=libiniphile-$(VERSION_major).so
LIBOBJECTS=input.o output.o ast.o
OBJECTS=initest-shared.o initest-static.o $(LIBOBJECTS)
LD_soname=

PUBLIC_HEADERS=ast.hpp astfwd.hpp declspec.hpp input.hpp metagram.hpp output.hpp

ARTIFACTS=$(OBJECTS) initest-static$(EXE) initest-shared$(EXE) \
	  *.exp *.lib *.manifest \
	  $(SONAME) *.a *.pc

EMBED_MANIFEST= \
	$(MT) $(MTFLAGS) -manifest $@.manifest -outputresource:$@;1

EXE=.exe

all: initest $(LIBINIPHILE_PC)

clean:
	$(RM_F) $(ARTIFACTS)

check: initest
	initest-static$(EXE)
	initest-shared$(EXE)

install: all
	if not exist $(DESTDIR)$(LIBDIR) \
	$(MKDIR_P) $(DESTDIR)$(LIBDIR)
	$(INSTALL) libiniphile.a $(DESTDIR)$(LIBDIR)\libiniphile.a
	$(INSTALL) libiniphile.lib $(DESTDIR)$(LIBDIR)\libiniphile.lib
	$(INSTALL) $(SONAME) $(DESTDIR)$(LIBDIR)\$(SONAME)
	if not exist $(DESTDIR)$(INCDIR) \
	$(MKDIR_P) $(DESTDIR)$(INCDIR)
	for %f in ($(PUBLIC_HEADERS)) do \
		$(INSTALL) %f $(DESTDIR)$(INCDIR)\%f

initest: initest-static$(EXE) initest-shared$(EXE)

libiniphile.lib: $(SONAME)

$(SONAME): $(LIBOBJECTS)
	$(LD) $(LDFLAGS) $(LD_shared) $(LD_soname) /IMPLIB:libiniphile.lib $(LD_o)$(SONAME) $(LIBOBJECTS) $(LDLIBS)

libiniphile.a: $(LIBOBJECTS)
	$(AR) $(ARFLAGS) $(AR_rc)libiniphile.a $(LIBOBJECTS)

initest-static$(EXE): initest-static.o libiniphile.a
	$(LD) $(LDFLAGS) $(LD_o)initest-static$(EXE) initest-static.o libiniphile.a $(LDLIBS)
	$(EMBED_MANIFEST)

initest-shared$(EXE): initest-shared.o libiniphile.lib
	$(LD) $(LDFLAGS) $(LD_o)initest-shared$(EXE) initest-shared.o libiniphile.lib $(LDLIBS)
	$(EMBED_MANIFEST)

COMPILE=$(CXX) $(CXXFLAGS) $(CXX_c) $(CXX_o)

initest-static.o: initest.cpp
	$(COMPILE)$@ initest.cpp

initest-shared.o: initest.cpp
	$(COMPILE)$@ initest.cpp -DINIPHILE_DLL

.cpp.o:
	$(COMPILE)$@ $< -DINIPHILE_DLL

initest-static.o: metagram.hpp input.hpp output.hpp ast.hpp
initest-shared.o: metagram.hpp input.hpp output.hpp ast.hpp
input.o: metagram.hpp input.hpp
output.o: metagram.hpp output.hpp ast.hpp
ast.o: metagram.hpp ast.hpp

.PHONY: all check clean initest
.SUFFIXES: .o
