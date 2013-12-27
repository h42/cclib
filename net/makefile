CPPFLAGS=-g -Wall
CC=g++

PROGS=jsockc jsocks http jftp demon jntp

all:$(PROGS)

http: http.cc jsock.h jsock.o
	g++ $(CPPFLAGS)  -o http  http.cc jsock.o  -l my

jsock.o jsock.h:jsock.cc
	awkinc <jsock.cc >jsock.h
	g++ -c $(CPPFLAGS) jsock.cc

jsockc:jsock.o jsock_c.cc
	g++ $(CPPFLAGS) jsock.o jsock_c.cc -ojsockc

jsocks:jsock.o jsock_s.cc
	g++ $(CPPFLAGS) jsock.o jsock_s.cc -ojsocks

jntp:jsock.o jntp.cc
	g++ $(CPPFLAGS) jsock.o jntp.cc -ojntp

## JFTP
jftp.o jftp.h:jftp.cc
	awkinc <jftp.cc >jftp.h
	g++ -c $(CPPFLAGS) jftp.cc

jftp:jftp.o jftp_t.cc jsock.o
	g++ $(CPPFLAGS) jftp.o jsock.o jftp_t.cc -ojftp

## INSTALL
install:jsock.o
	mkdir -p ../my
	cp -av jsock.h ../my/jsock.h
	cp -av jsock.o ../my/jsock.o

clean:
	-rm *.o $(PROGS)
