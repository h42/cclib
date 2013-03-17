CPPFLAGS=-g -Wall
CC=g++

OBJS=sql.o

all:ccinc $(OBJS)

ccinc:ccinc.c

sql.o:sql.cc
	ccinc <sql.cc >sql.h
	g++ -c $(CPPFLAGS) sql.cc -osql.o

clean:
	-rm *.o *.h $(PROGS) ccinc

install:
	-rm libjlib.a
	ar qs libjlib.a $(OBJS)
	install -v -ojerry -gjerry libjlib.a /usr/local/lib
	install -d -v /usr/local/include/jlib
	install -v -ojerry -gjerry *.h /usr/local/include/jlib
