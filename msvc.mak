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
LDFLAGS_SO=/DLL /IMPLIB:$(IMPORT_LIB)
LDLIBS=boost_unit_test_framework.lib
LDLIBS_SHARED=$(IMPORT_LIB)
MKDIR_P=mkdir
MT=mt.exe
MTFLAGS=/nologo
RM_F=del /f
INSTALL_PROGRAM=$(INSTALL)
INSTALL=copy

VERSION_major=0
VERSION_minor=1
VERSIONSTRING=$(VERSION_major).$(VERSION_minor)

COMPILE=$(CXX) $(CXXFLAGS) $(CXX_c) $(CXX_o)
DLL_LINKAGE=-DINIPHILE_DLL

LIBINIPHILE_PC=
CANONICAL=libiniphile.so
SONAME=libiniphile-$(VERSION_major).so
IMPORT_LIB=libiniphile.lib
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
	$(INSTALL_PROGRAM) libiniphile.a $(DESTDIR)$(LIBDIR)\libiniphile.a
	$(INSTALL_PROGRAM) $(SONAME) $(DESTDIR)$(LIBDIR)\$(SONAME)
	$(INSTALL_PROGRAM) $(IMPORT_LIB) $(DESTDIR)$(LIBDIR)\$(IMPORT_LIB)
	if not exist $(DESTDIR)$(INCDIR) \
	$(MKDIR_P) $(DESTDIR)$(INCDIR)
	for %f in ($(PUBLIC_HEADERS)) do \
		$(INSTALL) %f $(DESTDIR)$(INCDIR)\%f

initest: initest-static$(dot_exe) initest-shared$(dot_exe)

$(IMPORT_LIB): $(SONAME)

$(SONAME): $(LIBOBJECTS)
	$(LD) $(LDFLAGS) $(LDFLAGS_SO) $(LD_o)$(SONAME) $(LIBOBJECTS)

libiniphile.a: $(LIBOBJECTS)
	$(AR) $(ARFLAGS) $(AR_rc)libiniphile.a $(LIBOBJECTS)

initest-static$(dot_exe): initest-static.o libiniphile.a
	$(LD) $(LDFLAGS) $(LD_o)initest-static$(dot_exe) initest-static.o libiniphile.a $(LDLIBS)
	$(EMBED_MANIFEST)

initest-shared$(dot_exe): initest-shared.o
	$(LD) $(LDFLAGS) $(LD_o)initest-shared$(dot_exe) initest-shared.o $(LDLIBS_SHARED) $(LDLIBS)
	$(EMBED_MANIFEST)

initest-static.o: initest.cpp
	$(COMPILE)$@ initest.cpp

initest-shared.o: initest.cpp
	$(COMPILE)$@ initest.cpp $(DLL_LINKAGE)

.cpp.o:
	$(COMPILE)$@ $< $(DLL_LINKAGE)

initest-shared$(dot_exe): $(IMPORT_LIB)
initest-static.o: metagram.hpp input.hpp output.hpp ast.hpp
initest-shared.o: metagram.hpp input.hpp output.hpp ast.hpp
input.o: metagram.hpp input.hpp
output.o: metagram.hpp output.hpp ast.hpp
ast.o: metagram.hpp ast.hpp

.PHONY: all check clean initest
.SUFFIXES: .o
