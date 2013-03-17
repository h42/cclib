CPPFLAGS=-g -Wall
CC=g++

PROGS=jpd ttime

all:$(PROGS)

ttime:ttime.cc jtime.o

jtime.o:jtime.cc
	../awkinc <jtime.cc >jtime.h
	g++ -c $(CPPFLAGS) jtime.cc -ojtime.o

jpd:jpd.cc
	g++ $(CPPFLAGS) jpd.cc -ojpd

install:jpd
	install -o jerry -g jerry jpd /usr/local/bin/daysleft

clean:
	-rm *.o $(PROGS)
