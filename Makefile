CXX=g++44
CXXFLAGS=-std=c++0x -Wall -Werror -Wfatal-errors -I..
LDFLAGS=-L/usr/local/lib/gcc44 -rpath /usr/local/lib/gcc44
LDLIBS=

all: iniphile

clean:
	rm -f iniphile *.o

check: iniphile
	./iniphile < lf.ini

.PHONY: check clean
