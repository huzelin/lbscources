INCLUDE = -I../../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE) $(DEBUG_DEF) -std=c++11
LIBS = libgrid.a

all : clean $(LIBS)

clean:
	rm -rf *.o *.a

libgrid.a: lbs_bitmap.o lbs_hashtable.o lbs_nn_heap.o lbs_grid.o lbs_index.o
	ar cr $@ $^

.o : .cpp
	g++ $(CPPFLAGS) -c $^
