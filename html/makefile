
CPPFLAGS=-g -Wall -static

PROGS=jpd
#OBJS=jfile.o

.PHONY: ALL

ALL:$(OBJS) $(PROGS)

jfile.o:jfile.cc jfile.h
	awkinc <jfile.cc >jfile.h
	g++ -Wall -g -c jfile.cc -ojfile.o
jfile_t:jfile_t.cc jfile.o jstring.o

jpd:jpd.cc
	g++ $(CPPFLAGS) jpd.cc -ojpd

lexer:lexer.cc jstring.o jvec.cc
	g++ -Wall -g lexer.cc jstring.o -olexer

mime:mime.cc
	g++ -Wall -g mime.cc -omime

install:jpd
	install -m755 jpd /srv/http/cgi-bin/jpd

clean:
	-rm -v *.o $(PROGS)
