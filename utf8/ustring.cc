#include <iostream>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

using namespace std;

typedef unsigned char uchar;
typedef char * cptr;

//ccinclude
#ifndef _USTRING_H
#define _USTRING_H

class ustring {
public:
    ustring(const char *s, int ilen=0);
    //ustring(const int ilen=0);
    //ustring(const ustring &s);
    ~ustring();
    void disp() { if (len>0) puts((cptr)buf); else puts("");}
    void grow(int l);
private:
    uchar *buf;
    int len,ulen,size;
};
#endif
//ccinclude

//
// GROW
//
void ustring::grow(int l) {
    if (size<=0) buf=(uchar *)malloc(l);
    else buf=(uchar * )realloc(buf,l);
    if (!buf) len=size=0;
    size=l;
}

ustring::~ustring() {
    if (buf) free(buf);
}

ustring::ustring(const char *s, int isize) {
    int l=strlen(s);
    if (l+1>isize) isize=l+1;
    size=0; //req for initial malloc
    grow(isize); // size must be 0 before 1st grow
    if (size<isize) return;
    if (s) {
	int i;
	for (i=0; s[i]; i++) buf[i]=(uchar)s[i];
	buf[i]=0;
    }
    len=l;
}

int main() {
    ustring u1("hey");
    u1.disp();
    return 0;
}
