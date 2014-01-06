CPPFLAGS=-g -Wall -I .
CC=g++

PROGS=http jpd

%.o : %.cc
	g++ -c $(CLG) -o $@ $<

% : %.cc
	$(CC) $(CPPFLAGS) -o $@ $< -lmy

all:$(PROGS)


install:

clean:
	-rm *.o $(PROGS)
