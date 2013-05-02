
CPPFLAGS=-g -Wall -I ../lib -L ../lib

PROGS=jpd html

.PHONY: ALL

ALL:$(OBJS) $(PROGS)

jpd:jpd.cc
	g++ $(CPPFLAGS) jpd.cc -lmy -ojpd

html:html.cc
	g++ $(CPPFLAGS) html.cc -lmy -ohtml

lexer:lexer.cc jstring.o jvec.cc
	g++ -Wall -g lexer.cc jstring.o -olexer

mime:mime.cc
	g++ -Wall -g mime.cc -omime

install:jpd
	install -m755 jpd /srv/http/cgi-bin/jpd

clean:
	-rm -v *.o $(PROGS)
