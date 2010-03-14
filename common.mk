# vim: ts=8 noet ft=make

CANONICAL=libiniphile.so

COMPILE=$(CXX) $(CXXFLAGS) $(CXX_c) $(CXX_o)
LIBOBJECTS=input.o output.o ast.o
OBJECTS=initest-shared.o initest-static.o $(LIBOBJECTS)

PUBLIC_HEADERS=ast.hpp astfwd.hpp declspec.hpp input.hpp metagram.hpp output.hpp

ARTIFACTS=$(OBJECTS) initest-static$(dot_exe) initest-shared$(dot_exe) \
	  *.exp *.lib *.manifest \
	  $(SONAME) *.a *.pc

all: initest $(LIBINIPHILE_PC)

libs: libiniphile.a $(SONAME)

clean:
	$(RM_F) $(ARTIFACTS)

initest: initest-static$(dot_exe) initest-shared$(dot_exe)

$(SONAME): $(LIBOBJECTS)
	$(LD) $(LDFLAGS) $(LDFLAGS_SO) $(LD_o)$(SONAME) $(LIBOBJECTS)

libiniphile.a: $(LIBOBJECTS)
	$(AR) $(ARFLAGS) $(AR_o)libiniphile.a $(LIBOBJECTS)

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

.PHONY: all check clean initest libs
.SUFFIXES: .o
