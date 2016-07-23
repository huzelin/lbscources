INCLUDE = -I../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE) -std=c++11
LIBS = libidc.a

all : clean $(LIBS)

clean:
	rm -rf *.o $(LIBS)

genpb:
	protoc  mod-update.proto --cpp_out=./

libidc.a: mod-update.pb.o 
	ar cr $@ $^

.o : .cc
	g++ $(CPPFLAGS) -c $^
