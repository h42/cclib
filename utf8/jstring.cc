#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

//ccinclude
#ifndef _JSTRING_H
#define _JSTRING_H

class jstring {
public:
    jstring(const char *s,int ilen=0);
    jstring(const int ilen=0);
    jstring(const jstring &s);
    ~jstring();
    int center(int x=0);
    char *cstr() {return buf;};
    void display() {::puts(buf);}
    void log() {::printf("len=%d size=%d buf=%s\n",len,size,buf);}
    int find(const char *str,int x=0,int icase=0);
    void grow(int l);
    int  length() {return len;};
    jstring &operator=(const jstring &s);
    jstring &operator=(const char *s);
    jstring operator+(const jstring &s2);
    friend jstring operator+(const char *s,const jstring &s1);
    int printf(const char *fmt,...);
    int change(const char *,int x=0,int l1=0,int l2=0);
    int trim(int x=3);
    int triml() {return trim(1);}
    int trimr() {return trim(2);}
private:
    char *buf;
    short len,size;
} ;

#endif
//ccinclude

//
// Constructor / destructor
//
jstring::jstring(const char *s,int ilen) {
    int l=strlen(s);
    if (l+1>ilen) ilen=l+1;
    size=0; //req for initial malloc
    grow(ilen); // size must be 0 before 1st grow
    if (size<ilen) return;
    if (s) memcpy(buf,s,l+1);
    len=l;
}

jstring::jstring(int isize) {
    len=0;
    if (isize<=0) isize=80;
    size=0; //req for initial malloc
    grow(isize); // size must be 0 before 1st grow
    if (size<isize) return;
    buf[0]=0;
}

jstring::jstring(const jstring &s) {
    len=s.len;
    size=0; //req for initial malloc
    grow(s.size);
    if (size!=s.size) return;
    memcpy(buf,s.buf,len+1);
}

jstring::~jstring() {
    if (buf) free(buf);
}

//////////////
// CENTER
//////////////
int jstring::center(int x) {
    if (x<=0) x=size-1;
    if (size<=0 || x>=size-1) return -1;
    if (len<=0 || len==x) return 0;
    if (len<x) {
	int p1;
	p1=(x-len)/2;
	if (p1>0) {
	    memmove(buf+p1,buf,len);
	    memset(buf,' ',p1);
	}
	memset(buf+p1+len,' ',x-(p1+len));
    }
    buf[x]=0;
    len=x;
    return 0;
}
//
// FIND
//
int jstring::find(const char *str,int x,int icase) {
    if (x>=len) return -1;
    char *cp;
    cp=(!icase)?strstr(&buf[x],str):strcasestr(&buf[x],str);
    return (cp)?cp-buf:-1;
}

//
// GROW
//
void jstring::grow(int l) {
    if (size<=0) buf=(char *)malloc(l);
    else buf=(char *)realloc(buf,l);
    if (!buf) len=size=0;
    size=l;
}

///////////////////
// operator
///////////////////
jstring &jstring::operator=(const jstring &s) {
    if (&s == this) return *this;
    if (size<s.len+1) grow(s.len+1);
    if (size<s.len+1) return *this;
    if (s.size) memcpy(buf,s.buf,s.len+1);
    else if (size) buf[len=0]=0;
    len=s.len;
    return *this;
}

jstring &jstring::operator=(const char *str1) {
    int l=strlen(str1);
    if (size<l+1) grow(l+1);
    if (size<l+1) return *this;
    memcpy(buf,str1,l+1);
    len=l;
    return *this;
}

jstring jstring::operator+(const jstring &s2) {
    jstring s0;
    int l=len+s2.len;
    s0.buf=(char *)malloc(l+1);
    if (!s0.buf) return s0;
    s0.size=l+1;
    s0.len=l;
    if (len>0) memcpy(s0.buf,buf,len);
    if (s2.len>0) memcpy(&s0.buf[len],s2.buf,s2.len);
    s0.buf[l]=0; // incase s2.len=0
    return s0;
}

jstring operator+(const char *s1,const jstring &s2) {
    jstring s0;
    int s1l=strlen(s1);
    int l = s1l + s2.len;
    s0.buf=(char *)malloc(l+1);
    if (!s0.buf) return s0;
    s0.size=l+1;
    s0.len=l;
    if (s1l) memcpy(s0.buf,s1,s1l);
    if (s2.len) memcpy(&s0.buf[s1l],s2.buf,s2.len);
    s0.buf[l]=0;
    return s0;
}

//
// PRINTF
//
int jstring::printf(const char *fmt,...) {
    va_list ap;
    if (size==0) return -1;
    va_start(ap,fmt);
    int rc=vsnprintf(buf,size,fmt,ap);
    len = (rc < size) ? rc : size-1;
    va_end(ap);
    return rc;
}

//
// REPLACE
//
int jstring::change(const char *str,int x,int l1,int l2) {
    if (l2<=0) l2=strlen(str);
    if (l1<=0) l1=l2;
    if (len+l2-l1>=size) return -1;
    memmove(buf+x+l2,buf+x+l1,len-(x+l1));
    memmove(buf+x,str,l2);
    return 0;
}

//
// TRIM
//
int jstring::trim(int x) {
    if (len<=0) return -1;
    int i;
    if (x&1) {
	int p1=0;
	for (i=0;i<len;i++) if (isgraph(buf[i])) {
	    p1=i;
	    break;
	}
	if (p1) {
	    len-=p1;
	    memmove(buf,&buf[p1],len+1);
	}
    }
    if (x&2) {
	for (i=len-1;i>=0;i--) if (isgraph(buf[i])) {
	    len=i+1;
	    buf[len]=0;
	    break;
	}
    }
    return 0;
}

