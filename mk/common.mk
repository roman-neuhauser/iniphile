# vim: ts=8 noet ft=make

.PHONY: .all all check check-solink clean dist initest install libs
.PHONY: rpm
.SUFFIXES: .o .html .rest

.all: iniphile$(dot_exe) initest $(INIPHILE_PC)

libs: libiniphile.a $(SONAME)

docs: README.html

clean:
	$(RM_F) $(ARTIFACTS)

dist:
	hg archive -p $(DISTNAME) $(DISTNAME).tar.bz2

initest: initest-static$(dot_exe) initest-shared$(dot_exe)

$(SONAME): $(LIBOBJECTS)
	$(LD) $(LDFLAGS) $(LDFLAGS_SO) $(LD_o)$(SONAME) $(LIBOBJECTS)

libiniphile.a: $(LIBOBJECTS)
	$(AR) $(ARFLAGS) $(AR_o)libiniphile.a $(LIBOBJECTS)

iniphile$(dot_exe): iniphile.o libiniphile.a
	$(LD) $(LDFLAGS) $(LDFLAGS_static) $(LD_o)iniphile$(dot_exe) iniphile.o libiniphile.a
	$(EMBED_MANIFEST)

initest-static$(dot_exe): initest-static.o libiniphile.a
	$(LD) $(LDFLAGS) $(LDFLAGS_static) $(LD_o)initest-static$(dot_exe) initest-static.o libiniphile.a $(LDLIBS)
	$(EMBED_MANIFEST)

initest-shared$(dot_exe): initest-shared.o $(IMPORT_LIB) $(SONAME)
	$(LD) $(LDFLAGS) $(LDFLAGS_shared) $(LD_o)initest-shared$(dot_exe) initest-shared.o $(LDLIBS_SHARED) $(LDLIBS)
	$(EMBED_MANIFEST)

initest-static.o: initest.cpp
	$(COMPILE)$@ initest.cpp

initest-shared.o: initest.cpp
	$(COMPILE)$@ initest.cpp $(DLL_LINKAGE)

iniphile.o: iniphile.cpp version.hpp
	$(COMPILE)$@ iniphile.cpp

version-hpp.o: version-hpp.cpp
	$(COMPILE)$@ version-hpp.cpp

version-hpp$(dot_exe): version-hpp.o
	$(LD) $(LDFLAGS) $(LDFLAGS_static) $(LD_o)$@ version-hpp.o
	$(EMBED_MANIFEST)

version.hpp: version-hpp$(dot_exe)
	./version-hpp $@ $(INIPHILE_DEFINES)

.rest.html:
	$(RST2HTML) $< $@

.cpp.o:
	$(COMPILE)$@ $< $(DLL_LINKAGE)

initest-static.o: metagram.hpp input.hpp output.hpp ast.hpp
initest-shared.o: metagram.hpp input.hpp output.hpp ast.hpp
input.o: metagram.hpp input.hpp
output.o: metagram.hpp output.hpp ast.hpp
ast.o: metagram.hpp ast.hpp

