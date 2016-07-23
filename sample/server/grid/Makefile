ifeq ($(VERSION), debug)
	DEBUG_DEF = -g -D_DEBUG
else
ifeq ($(VERSION), release)
	DEBUG_DEF = -O2
else
	DEBUG_DEF = -g -O2
endif
endif

INCLUDE = -I../../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE) $(DEBUG_DEF) -std=c++11
LIBS = libgrid.a

all : clean $(LIBS)

clean:
	rm -rf *.o *.a

libgrid.a: index.o hash-table.o nn-heap.o grid.o 
	ar cr $@ $^

.o : .cpp
	g++ $(CPPFLAGS) -c $^
