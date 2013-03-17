CPPFLAGS=-g -Wall
CC=g++

PROGS=ccinc jpd ustring

all:$(PROGS)

jpd:jpd.cc
	g++ $(CPPFLAGS) -o $@ $<


clean:
	-rm *.o $(PROGS)
