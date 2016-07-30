#Macros
SUBDIRS = grid 

INCLUDE = -I../ \

CPPFLAGS = -Wall -fPIC $(INCLUDE) $(DEBUG_DEF) -std=c++11
LIBS = ../common/libcommon.a \
       ../proto/libidc.a \
       ./grid/libgrid.a \
       /usr/local/lib/libevent.a \
       /usr/local/lib/libevhtp.a \
       /usr/local/lib/libevent_core.a \
       /usr/local/lib/libjansson.a -lpthread -lprotobuf
EXE = server

all : clean subbuild $(EXE)

subbuild :
	@for X in $(SUBDIRS); \
        do \
	  cd $$X; make ; cd -; \
	done

clean :
	rm -rf $(EXE) *.o
	@for X in $(SUBDIRS); \
	do \
	  cd $$X; make clean; cd -; \
	done

## server
server : query.o server.o main.o
	g++ $(CPPFLAGS) -o $@ $^ $(LIBS)

.o : .cpp
	g++ $(CPPFLAGS) -c $^
