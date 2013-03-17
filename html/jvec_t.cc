#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "jvec.cc"
#include "jstring.h"

//template class jvec<int>;

using namespace std;

int cnt=5;

class xxx {
public:
    int x,y,z;
    xxx() { puts("contructing xxx"); x=0; y=0; z=0; }
    ~xxx() { puts("deconstructing"); }
    void inc(int i) { x+=i; y+=i; z+=i; }
};

/*
int sub1() {
    int i;
    jvec<int> v1(cnt);
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
    jvec<xxx> v1(cnt);
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
    jvec<int> iv;
    iv.grow(32);
    printf("size=%d\n",iv.size());
}

void bm2(jvec<long> &v) {
    int i,l=v.size();
    time_t t1;

    t1=time(0);
    long *ll = v.buf;
    for (i=0;i<200000000;i++) {
	ll[i%l] += 1;
    }
    printf("et=%d\n",(int)(time(0)-t1));

    t1=time(0);
    for (i=0;i<200000000;i++) {
	v[i%l] += 1;
    }
    printf("et=%d\n",(int)(time(0)-t1));
}

void bm() {
    jvec<long> v(1000);
    for (int i=0; i<v.size(); i++) {
	v[i]=i;
    }
    bm2(v);
}
*/

#include <string>
void bm() {
    int i,n=3;
    //jvec<xxx> iv;
    jvec<int> iv2;
    jvec<jstring> iv(5);
    jstring s("hey");
    try {
	for (i=0; i<n; i++) {
	    //iv2.push(i);
	    iv.push(s);
	}
	printf("size=%d\n",iv2.size());
	//iv.pop().display();
    }
    catch (const char *e) {
	puts(e);
    }
}

int main() {
    bm();
    //sub1();
    //putchar(10);
    //sub2();
    //sub3();
    return 0;
}
