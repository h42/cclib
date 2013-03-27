#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "my/vec.cc"

//#include "jstring.h"
//template class vec<int>;

int cnt=5;

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

class xxx {
public:
    int x,y,z;
    xxx() { puts("contructing xxx"); x=0; y=0; z=0; }
    ~xxx() { puts("deconstructing"); }
    void inc(int i) { x+=i; y+=i; z+=i; }
};

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
    printf("sub3 size=%d\n",iv.size());
}

void trev() {
    int n=5;
    vec<int> vi(n);
    for (int i=0;i<n;i++) vi[i]=i;
    vi.reverse(n);
    for (int i=0;i<n;i++) printf("vi[%d]=%d %p\n",i,vi[i],&vi[i]);
}

void mmsort() {
    const int N = 200000;
    vec<int> v(N);
    srand(7);
    for (int i=0;i<v.size();i++) v[i]=rand() % 1000000;

    for (int i=0; i<5; i++) printf("%d\n",v[i]);
    v.msort();
    for (int i=0; i<5; i++) printf("%d\n",v[i]);
}

int main() {
    /*
    sub1();
    putchar(10);
    sub2();
    putchar(10);
    sub3();
    putchar(10);
    trev();
    putchar(10);
    */
    mmsort();
    return 0;
}
