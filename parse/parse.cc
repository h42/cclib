#include <stdio.h>
#include <malloc.h>
#include <ctype.h>

//ccinclude
#ifndef _CLASS_H
#define _CLASS_H

#ifndef _VEC_H
#include <my/vec.h>
#endif

#ifndef _STR_H
#include <my/str.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

class parse {
public:
    parse(const char *s="");
    void reset(const char *s);
    bool eol();
    bool getany(str s, int count=1);
    bool getchar(char x, str s, int count=1);
    bool getint(int *);
    bool getline(str &s);
    bool getqstr(str &s);
    bool getuntil(int (*)(int), str &s);
    bool getword(str &s);
    bool getwhile(int (*)(int), str &s);
    bool lines(vec<str> &v);
    bool spaces();
    bool words(vec<str> &v);
private:
    const char *zbuf;
    int zp1;
    bool eod;
};

const int EOD = -99;

#endif
//ccinclude

parse::parse(const char *s) {
    reset(s);
}

void parse::reset(const char *s) {
    zbuf=s;;
    zp1=0;
}

bool parse::getany(str s, int len) {
    if (zbuf[zp1]==0) return false;
    s="";
    if (len==1)  s.append(zbuf[zp1++]);
    int c,i;
    for (i=0; i<len; i++) {
        c=zbuf[zp1++];
        if (c==0) {
            zp1--;
            break;
        }
        else s.append(c);
    }
    return true;
}

bool parse::getchar(char x, str s, int len) {
    s="";
    int c,i;
    if ((c=zbuf[zp1])==0) return EOD;
    if (len==1) {
        if (c==x) {
            s.append(c);
            zp1++;
            return true;
        }
        else return false;
    }
    for (i=0; i<len; i++) {
        c=zbuf[zp1++];
        if (c==0) {
            zp1--;
            break;
        }
        else s.append(c);
    }
    return  (i>0) ? true : false ;
}

bool parse::eol() {
    int c=zbuf[zp1];
    if (c==10 || c==13) {
        zp1++;
         if (c==13 && zbuf[zp1+1] == 10) zp1++;
        return true;
    }
    return false;
}

bool parse::getint(int *ii) {
    int i,rc;
    *ii=0;
    if (zbuf[zp1]==0) return EOD;
    for (i=zp1; zbuf[i]; i++) {
	if (!isdigit(zbuf[i])) break;
	*ii = *ii * 10 + zbuf[i]-'0';
    }
    rc=i-zp1;
    zp1=i;
    return (rc>0) ? true : false;
}

bool parse::getline(str &s) { //get from cur pos to eol
    int i,c;
    if (zbuf[zp1]==0) return false;
    s="";
    for (i=zp1; zbuf[i]; i++) {
        c=zbuf[i];
        if (c==10 || c==13 || c==0) {
            zp1 = i + (c ? 1 : 0); // do not increment over EOD
            if (c==13 && zbuf[zp1] == 10) zp1++;
            break;
        }
        else s.append(c);
    }
    return true; // should not get here
}

bool parse::getqstr(str &s) {
    if (zbuf[zp1]==0) return false;
    int i,c;
    s="";
    if (zbuf[zp1] != '"') return false;
    for (i=zp1+1;; i++) {
        c=zbuf[i];
        if (c=='"') break;
        if (c==10 || c==13 || !c) return false;
        s.append(c);
    }
    zp1=i + 1;
    return true;
}

bool parse::getuntil(int (*func)(int), str &s) {
    int i,c;
    s="";
    if (zbuf[zp1]==0) return false;
    for (i=zp1; ; i++) {
        c=zbuf[i];
        if (func(c) || !c) break;
        s.append(zbuf[i]);
    }
    zp1=i;
    return true;
}

bool parse::getwhile(int (*func)(int), str &s) {
    int i,c;
    if (zbuf[zp1]==0) return false;
    s="";
    for (i=zp1; ; i++) {
        c=zbuf[i];
        if (!func(c) || !c) break;
        s.append(c);
    }
    zp1=i;
    return true;
}

bool parse::getword(str &s) {
    if (zbuf[zp1]==0) return false;
    int i,c,rc;
    s="";
    for (i=zp1; ; i++) {
        c=zbuf[i];
        if (isspace(c) || !c) break;
        s.append(c);
    }
    rc=i-zp1;
    zp1=i;
    return (rc>0) ? true : false;
}

bool parse::lines(vec<str> &v) {
    if (zbuf[zp1]==0) return false;
    str s;
    v.sp(0);
    while (getline(s)) v.push(s);
    return true;
}

bool parse::spaces() {
    int i;
    if (zbuf[zp1]==0) return false;
    for (i=zp1; ; i++) if (!isspace(zbuf[i])) break;
    zp1=i;
    return true;
}

bool parse::words(vec<str> &v) {
    str s;
    if (zbuf[zp1]==0) return false;
    v.sp(0);
    spaces();
    while (getword(s)) {
        v.push(s);
        spaces();
    }
    return true;
}
