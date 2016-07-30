INCLUDE = -I../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE) $(DEBUG_DEF) -std=c++11
LIBS = libsimulator.a ../common/libcommon.a ../proto/libidc.a -lpthread -lcurl -lprotobuf
TEST = taxi-data-queue_test
EXE = simulator

all : clean $(LIB) $(EXE) $(TEST)
	./taxi-data-queue_test

clean:
	rm -rf *.o *.a $(EXE) $(TEST)

## simulator
simulator: simulator.o $(LIBS)
	g++ $(CPPFLAGS) -o $@ $^ $(LIBS)

## test unit
taxi-data-queue_test: taxi-data-queue_test.o $(LIBS)
	g++ $(CPPFLAGS) -o $@ $^ $(LIBS)

libsimulator.a: taxi-data-queue.o taxi-data-extraction.o controller.o 
	ar cr $@ $^

.o : .cpp
	g++ $(CPPFLAGS) -c $^
