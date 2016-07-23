INCLUDE = -I../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE)

LIB= libcommon.a

all: clean $(LIB)

clean:
	rm -r -f *.o *.so.* $(LIB)

libcommon.a: log.o properties.o thread-fn.o
	ar cr $@ $^
.o: .cpp
	g++ ${CPPFLAGS} -c $^
