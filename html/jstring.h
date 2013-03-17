#ifndef _JSTRING_H
#define _JSTRING_H

class jstring {
public:
    jstring(const char *s,int ilen=0);
    jstring(const int ilen=0);
    jstring(const jstring &s);
    ~jstring();
    int center(int x=0);
    char *cstr() {return zbuf;};
    void display() {::puts(zbuf);}
    void log() {::printf("len=%d size=%d buf=%s\n",zlen,zsize,zbuf);}
    int find(const char *str,int x=0,int icase=0);
    void grow(int l);
    int  length() {return zlen;};
    jstring &concat(const jstring &s);
    jstring &concat(char c);
    jstring &copy(const jstring &s);
    jstring &copy(const char *s);
    jstring &operator=(const jstring &s);
    jstring &operator=(const char *s);
    jstring operator+(char c);
    jstring operator+(const jstring &s2);
    friend jstring operator+(const char *s,const jstring &s1);
    int format(const char *fmt,...);
    int change(const char *,int x=0,int l1=0,int l2=0);
    int trim(int x=3);
    int triml() {return trim(1);}
    int trimr() {return trim(2);}
private:
    char *zbuf;
    short zlen,zsize;
} ;

#endif
