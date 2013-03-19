CPPFLAGS=-g -Wall -I .
CC=g++

CLASS=str

PROGS=jpd

%.o : %.cc
	g++ -c $(CLG) -o $@ $<

% : %.cc
	$(CC) $(CPPFLAGS) -o $@ $<

all:$(PROGS)

jpd:jpd.cc $(CLASS).o
	g++ $(CPPFLAGS) jpd.cc $(CLASS).o -ojpd

$(CLASS).o:$(CLASS).cc
	awkinc < $(CLASS).cc  >$(CLASS).h
	g++ -c $(CPPFLAGS) $(CLASS).cc -o $(CLASS).o

install:
	install -m644 $(CLASS).cc ../lib

clean:
	-rm *.o $(PROGS)
