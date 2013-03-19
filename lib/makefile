CPPFLAGS=-g -Wall
CC=g++

OBJS=str.o
INCS=str.h vec.cc

%.o : %.cc
	awkinc < $<  > $(@:.o=.h)
	$(CC) -c $(CPPFLAGS) -o $@ $<

all:$(OBJS)

clean:
	-rm *.o *.h $(PROGS)

install:
	-rm libmy.a
	ar qs libmy.a $(OBJS)
	install -v -ojerry -gjerry libmy.a /usr/local/lib
	install -d -v /usr/local/include/my
	install -v -ojerry -gjerry $(INCS) /usr/local/include/my
