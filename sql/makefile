CPPFLAGS=-g -Wall #-static
CC=g++

PROGS=ccinc jpd jpd2

all:$(PROGS)

sql.o:sql.cc
	ccinc <sql.cc >sql.h
	g++ -c $(CPPFLAGS) sql.cc -osql.o

jpd:jpd.cc sql.o
	g++ $(CPPFLAGS) jpd.cc sql.o -lsqlite3 -ojpd

jpd2:jpd2.cc
	g++ $(CPPFLAGS) jpd.cc -ljlib -lsqlite3 -ojpd2

clean:
	-rm *.o $(PROGS)
