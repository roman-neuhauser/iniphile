# vim: ts=8 noet ft=make

VERSION_major=0
VERSION_minor=5.2
VERSIONSTRING=$(VERSION_major).$(VERSION_minor)
DISTNAME=iniphile-$(VERSIONSTRING)

CANONICAL=libiniphile.so

INIPHILE_DEFINES=\
	INIPHILE_DISTNAME="$(DISTNAME)" \
	INIPHILE_VERSIONSTRING="$(VERSIONSTRING)"

COMPILE=$(CXX) $(CXXFLAGS) $(CXX_c) $(CXX_o)
LIBOBJECTS=input.o output.o ast.o
OBJECTS=iniphile.o initest-shared.o initest-static.o version-hpp.o \
	$(LIBOBJECTS)

PUBLIC_HEADERS=ast.hpp astfwd.hpp declspec.hpp input.hpp \
	metagram.hpp output.hpp version.hpp

ARTIFACTS=$(OBJECTS) initest-static$(dot_exe) initest-shared$(dot_exe) \
	  *.exp *.lib *.manifest \
	  iniphile$(dot_exe) \
	  version.hpp \
	  version-hpp$(dot_exe) \
	  $(CANONICAL) $(SONAME) *.a *.pc

