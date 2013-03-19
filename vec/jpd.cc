#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "vec.cc"
//#include "jstring.h"

//template class vec<int>;

using namespace std;

int cnt=5;

class xxx {
public:
    int x,y,z;
    xxx() { puts("contructing xxx"); x=0; y=0; z=0; }
    ~xxx() { puts("deconstructing"); }
    void inc(int i) { x+=i; y+=i; z+=i; }
};

int sub1() {
    int i;
    vec<int> v1(cnt);
    for (i=0;i<cnt;i++) {
	v1[i]=i;
    }
    try {
	for (i=0;i<cnt+1;i++) {
	    printf("v1[%d]=%d\n",i,v1[i]);
	}
    }
    catch (const char *msg) {
	printf("*** sub1 exception - %s\n",msg);
    }

    return 0;
}

int sub2() {
    int i;
    vec<xxx> v1(cnt);
    for (i=0;i<cnt;i++) {
	v1[i].inc(i);
    }
    try {
	for (i=0;i<cnt+1;i++) {
	    printf("v1[%d]=%d %d %d\n", i, v1[i].x, v1[i].y, v1[i].z);
	}
    }
    catch (const char *msg) {
	printf("*** sub2 exception - %s\n",msg);
    }

    return 0;
}

void sub3() {
    vec<int> iv;
    iv.grow(32);
    printf("size=%d\n",iv.size());
}

int main() {
    sub1();
    putchar(10);
    sub2();
    putchar(10);
    sub3();
    return 0;
}
