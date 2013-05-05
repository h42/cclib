#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "my/tup.h"
#include <my/str.h>

void t2() {
    tup<int,str> t(1,"hey");
    t.snd().display();
    t.snd()="now";
    t.snd().display();
    t.fst()=17;
    printf("fst=%d\n",t.fst());

    tup<int,str> t2;
    t2.fst()=12;
    t2.snd()="we're number two";
    printf("fst=%d 2nd=%s\n",t2.fst(),t2.snd().cstr());
}

int main() {
    t2();
    return 0;
    tup<int,double> t(12,15.7);
    tup<const char *, int> t2("123",123);
    printf("fst=%d snd=%.2f\n",t.fst(),t.snd());
    return 0;
}
