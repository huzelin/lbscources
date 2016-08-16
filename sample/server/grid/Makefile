INCLUDE = -I../../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE) $(DEBUG_DEF) -std=c++11
LIBS = libgrid.a
EXE = hashtable_test

all : clean $(LIBS) $(EXE)

clean:
	rm -rf *.o *.a

hashtable_test: lbs_hashtable_test.o
	g++ $(CPPFLAGS) -o $@ $^ $(LIBS)

libgrid.a: lbs_bitmap.o lbs_hashtable.o lbs_nn_heap.o lbs_grid.o lbs_index.o
	ar cr $@ $^

.o : .cpp
	g++ $(CPPFLAGS) -c $^
