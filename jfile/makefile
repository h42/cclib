CPPFLAGS=-g -Wall -I . -I ../str
CC=g++

CLASS=jfile

PROGS=jpd

%.o : %.cc
	g++ -c $(CLG) -o $@ $<

% : %.cc
	$(CC) $(CPPFLAGS) -o $@ $<

all:$(PROGS)

jpd:jpd.cc $(CLASS).o
	g++ $(CPPFLAGS) jpd.cc $(CLASS).o ../str/str.o -ojpd

$(CLASS).o:$(CLASS).cc
	awkinc < $(CLASS).cc  >$(CLASS).h
	g++ -c $(CPPFLAGS) $(CLASS).cc -o $(CLASS).o

install:
	install -m644 $(CLASS).cc ../lib

clean:
	-rm *.o $(PROGS)
