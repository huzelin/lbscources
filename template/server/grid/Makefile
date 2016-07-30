INCLUDE = -I../../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE) $(DEBUG_DEF) -std=c++11
LIBS = libgrid.a

all : clean $(LIBS)

clean:
	rm -rf *.o *.a

libgrid.a: lbs_index.o
	ar cr $@ $^

.o : .cpp
	g++ $(CPPFLAGS) -c $^
