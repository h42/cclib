CPPFLAGS=-g -Wall -std=c++11

ALL:jpd html

jpd:jpd.cc
html:html.cc

install:jpd
	install -v -m755 -o http -g http jpd /srv/http/cgi-bin/jpd

