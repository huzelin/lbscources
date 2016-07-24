#Macros
SUBDIRS = common proto simulator server

#Actions
all : clean
	@for X in $(SUBDIRS); \
        do \
	  cd $$X; make ; cd -; \
	done

clean :
	@for X in $(SUBDIRS); \
	do \
	  cd $$X; make clean; cd -; \
	done

