
PROGS=lexer jpd jvec_t jfile_t mime
OBJS=jstring.o jfile.o

.PHONY: ALL

ALL:ccinc $(OBJS) $(PROGS)

ccinc:ccinc.c

jstring.o:jstring.cc jstring.h
	ccinc <jstring.cc >jstring.h
	g++ -Wall -g -c jstring.cc -ojstring.o
jstring_t:jstring_t.cc jstring.o

jfile.o:jfile.cc jfile.h
	ccinc <jfile.cc >jfile.h
	g++ -Wall -g -c jfile.cc -ojfile.o
jfile_t:jfile_t.cc jfile.o jstring.o

jpd:jpd.cc jstring.o jvec.cc
	g++ -Wall -g jpd.cc jstring.o -ojpd

lexer:lexer.cc jstring.o jvec.cc
	g++ -Wall -g lexer.cc jstring.o -olexer

jvec_t:jvec_t.cc jstring.o jvec.cc
	g++ -Wall -g jvec_t.cc jstring.o -ojvec_t

mime:mime.cc
	g++ -Wall -g mime.cc -omime

clean:
	-rm -v *.o $(PROGS)
