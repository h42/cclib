CPPFLAGS=-g -Wall
CC=g++

OBJS=vec.o str.o jfile.o jdbm.o
INCS=str.h vec.h jfile.h jdbm.h

%.o : %.cc
	awkinc < $<  > $(@:.o=.h)
	$(CC) -c $(CPPFLAGS) -o $@ $<

all:$(OBJS)

clean:
	-rm *.o *.h $(PROGS)

install:$(OBJS)
	-rm libmy.a
	ar qs libmy.a $(OBJS)
	ar -t libmy.a
	install -v -ojerry -gjerry libmy.a /usr/local/lib
	install -d -v /usr/local/include/my
	install -v -ojerry -gjerry $(INCS) /usr/local/include/my
