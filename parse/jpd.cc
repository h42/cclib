#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../str/str.h"
#include "parse.h"

const char * xxx=
    "GET /form?q=123 HTTP/1.1\n"
    "Host: localhost:8080\n"
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:25.0) Gecko/20100101 Firefox/25.0\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate\n"
    "Cookie: jpdform=jpdformdata\r\n"
    "Connection: keep-alive\n"
    "Content-Length: 123\n"
    "\n"
    "this is my data"
    ;

void test4() {
    str s;
    int state=0, rc;
    parse p(xxx);
    parse p2;
    vec<str> v;
    while(1) {
        if (state==0) {
            rc=p.getline(s);
            if (rc<0) return;
            if (s.length()==0) {
                state=2;
                continue;
            }
            p2.reset(s.cstr()); // CAREFUL s.cstr() pointer can be reallocate and go bad"
            printf("len=%d state=%d - %s\n", s.length(), state, s.cstr());
            p2.words(v);
            if (v.sp()<2) continue;
            if (memcmp (v[0].cstr(),"Content-Len",11)==0)
                printf("got len - %s\n", v[1].cstr());
        }
        else if (state==2) {
            rc=p.getline(s);
            s.display();
            return;
        }
    }
}

void test1() {
    str s;
    parse p1("123 hey now");
    int rc,i;
    rc=p1.getint(&i);
    printf("rc=%d i=%d\n",rc,i);
    rc=p1.spaces();
    printf("rc=%d\n",rc);
    rc=p1.getword(s);
    printf("rc=%d s=%s\n",rc,s.cstr());
    rc=p1.spaces();
    printf("rc=%d\n",rc);
    rc=p1.getword(s);
    printf("rc=%d s=%s\n",rc,s.cstr());

    p1.reset("the time is now");
    rc=p1.getuntil(isspace,s);
    printf("rc=%d s=%s\n",rc,s.cstr());
    rc=p1.getwhile(isspace,s);
    rc=p1.getuntil(isspace,s);
    printf("rc=%d s=%s\n",rc,s.cstr());
}


void test2() {
    str s;
    const char * h="GET /form?q=123 HTTP/1.1";
    parse p1(h);
    p1.getword(s);
    s.display();
    p1.spaces();
    p1.getword(s);
    s.display();
    p1.spaces();
    p1.getword(s);
    s.display();

    puts("");
    vec<str> v;
    v.sp(0);
    p1.reset(h);
    p1.words(v);
    for (int i=0; i<v.sp(); i++) v[i].display();
}

void test3() {
    str s;
    parse p(xxx);
    int rc=0;
    while (rc>=0) {
        rc=p.getline(s);
        s.display();
    }

    puts("");
    p.reset(xxx);
    vec<str> v;
    p.lines(v);
    for (int i=0; i<v.sp(); i++) {
        v[i].display();
    }
}

int main() {
    test4();
    return 0;
}
