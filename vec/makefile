CPPFLAGS=-O2 -g -Wall -I .
CC=g++

CLASS=vec

PROGS=jpd sort

%.o : %.cc
	g++ -c $(CLG) -o $@ $<

% : %.cc
	$(CC) $(CPPFLAGS) -o $@ $<

all:$(PROGS)

jpd:jpd.cc $(CLASS).cc
	awkinc < $(CLASS).cc > $(CLASS).h
	g++ $(CPPFLAGS) jpd.cc -ojpd

sort:sort.cc
	g++ $(CPPFLAGS) sort.cc -osort

install:
	install -m644 $(CLASS).cc ../my

clean:
	-rm *.o $(PROGS)
