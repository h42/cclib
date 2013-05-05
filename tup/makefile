CPPFLAGS=-O2 -g -Wall -I .
CC=g++

CLASS=tup

PROGS=jpd

%.o : %.cc
	g++ -c $(CLG) -o $@ $<

% : %.cc
	$(CC) $(CPPFLAGS) -o $@ $<

all:$(PROGS)

jpd:jpd.cc $(CLASS).cc
	awkinc < $(CLASS).cc > $(CLASS).h
	g++ $(CPPFLAGS) jpd.cc -lmy -ojpd

install:
	install -m644 $(CLASS).cc ../lib

clean:
	-rm *.o $(PROGS)
