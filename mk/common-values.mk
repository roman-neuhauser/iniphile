# vim: ts=8 noet ft=make

VERSION_major=0
VERSION_minor=1
VERSIONSTRING=$(VERSION_major).$(VERSION_minor)

CANONICAL=libiniphile.so

COMPILE=$(CXX) $(CXXFLAGS) $(CXX_c) $(CXX_o)
LIBOBJECTS=input.o output.o ast.o
OBJECTS=initest-shared.o initest-static.o $(LIBOBJECTS)

PUBLIC_HEADERS=ast.hpp astfwd.hpp declspec.hpp input.hpp metagram.hpp output.hpp

ARTIFACTS=$(OBJECTS) initest-static$(dot_exe) initest-shared$(dot_exe) \
	  *.exp *.lib *.manifest \
	  $(SONAME) *.a *.pc

