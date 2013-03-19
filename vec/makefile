CPPFLAGS=-g -Wall -I .
CC=g++

CLASS=vec

PROGS=jpd

%.o : %.cc
	g++ -c $(CLG) -o $@ $<

% : %.cc
	$(CC) $(CPPFLAGS) -o $@ $<

all:$(PROGS)

jpd:jpd.cc $(CLASS).cc
	g++ $(CPPFLAGS) jpd.cc -ojpd

install:
	install -m644 $(CLASS).cc ../lib

clean:
	-rm *.o $(PROGS)
