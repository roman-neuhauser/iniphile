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
LDFLAGS=/nologo /INCREMENTAL:NO $(LD_L)$(BOOST_LIBDIR)
LDFLAGS_SO=/DLL /IMPLIB:libiniphile.lib
LDLIBS=boost_unit_test_framework.lib
MKDIR_P=mkdir
MT=mt.exe
MTFLAGS=/nologo
RM_F=del /f
INSTALL=copy

VERSION_major=0
VERSION_minor=1
VERSION_string=$(VERSION_major).$(VERSION_minor)

COMPILE=$(CXX) $(CXXFLAGS) $(CXX_c) $(CXX_o)

LIBINIPHILE_PC=
CANONICAL=libiniphile.so
SONAME=libiniphile-$(VERSION_major).so
LIBOBJECTS=input.o output.o ast.o
OBJECTS=initest-shared.o initest-static.o $(LIBOBJECTS)

PUBLIC_HEADERS=ast.hpp astfwd.hpp declspec.hpp input.hpp metagram.hpp output.hpp

ARTIFACTS=$(OBJECTS) initest-static$(dot_exe) initest-shared$(dot_exe) \
	  *.exp *.lib *.manifest \
	  $(SONAME) *.a *.pc

EMBED_MANIFEST= \
	$(MT) $(MTFLAGS) -manifest $@.manifest -outputresource:$@;1

dot_exe=.exe

all: initest $(LIBINIPHILE_PC)

clean:
	$(RM_F) $(ARTIFACTS)

check: initest
	initest-static$(dot_exe)
	initest-shared$(dot_exe)

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

initest: initest-static$(dot_exe) initest-shared$(dot_exe)

libiniphile.lib: $(SONAME)

$(SONAME): $(LIBOBJECTS)
	$(LD) $(LDFLAGS) $(LDFLAGS_SO) $(LD_o)$(SONAME) $(LIBOBJECTS)

libiniphile.a: $(LIBOBJECTS)
	$(AR) $(ARFLAGS) $(AR_rc)libiniphile.a $(LIBOBJECTS)

initest-static$(dot_exe): initest-static.o libiniphile.a
	$(LD) $(LDFLAGS) $(LD_o)initest-static$(dot_exe) initest-static.o libiniphile.a $(LDLIBS)
	$(EMBED_MANIFEST)

initest-shared$(dot_exe): initest-shared.o
	$(LD) $(LDFLAGS) $(LD_o)initest-shared$(dot_exe) initest-shared.o libiniphile.lib $(LDLIBS)
	$(EMBED_MANIFEST)

initest-static.o: initest.cpp
	$(COMPILE)$@ initest.cpp

initest-shared.o: initest.cpp
	$(COMPILE)$@ initest.cpp -DINIPHILE_DLL

.cpp.o:
	$(COMPILE)$@ $< -DINIPHILE_DLL

initest-shared$(dot_exe): libiniphile.lib
initest-static.o: metagram.hpp input.hpp output.hpp ast.hpp
initest-shared.o: metagram.hpp input.hpp output.hpp ast.hpp
input.o: metagram.hpp input.hpp
output.o: metagram.hpp output.hpp ast.hpp
ast.o: metagram.hpp ast.hpp

.PHONY: all check clean initest
.SUFFIXES: .o
