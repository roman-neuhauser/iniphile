# vim: ts=8 noet ft=make

VERSION_major=0
VERSION_minor=1
VERSIONSTRING=$(VERSION_major).$(VERSION_minor)
DISTNAME=iniphile-$(VERSIONSTRING)

CANONICAL=libiniphile.so

INIPHILE_CXXFLAGS=\
	-DINIPHILE_DISTNAME="$(DISTNAME)" \
	-DINIPHILE_VERSIONSTRING="$(VERSIONSTRING)"
INIPHILE_CXXFLAGS=

COMPILE=$(CXX) $(CXXFLAGS) $(INIPHILE_CXXFLAGS) $(CXX_c) $(CXX_o)
LIBOBJECTS=input.o output.o ast.o
OBJECTS=iniphile.o initest-shared.o initest-static.o $(LIBOBJECTS)

PUBLIC_HEADERS=ast.hpp astfwd.hpp declspec.hpp input.hpp metagram.hpp output.hpp

ARTIFACTS=$(OBJECTS) initest-static$(dot_exe) initest-shared$(dot_exe) \
	  *.exp *.lib *.manifest \
	  iniphile$(dot_exe) \
	  $(CANONICAL) $(SONAME) *.a *.pc

