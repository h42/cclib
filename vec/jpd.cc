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
    char *zbuf;
    xxx() { puts("contructing xxx"); x=0; y=0; z=0; zbuf=(char *)malloc(80);}
    ~xxx() { puts("deconstructing"); free(zbuf); }
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
            printf("v1[%d]=%d %d %d\n", i, v1.get(i).x, v1[i].y, v1[i].z);
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
    iv.resize(32);
    printf("sub3 size=%d\n",iv.size());
}

void trev() {
    int n=5;
    vec<int> vi(n);
    for (int i=0;i<n;i++) vi[i]=i;
    vi.reverse(n);
    for (int i=0;i<n;i++) printf("vi[%d]=%d %p\n",i,vi[i],&vi[i]);
}

void test_mergesort() {
    const int N = 1000000;
    vec<int> v(N);
    srand(7);

    for (int i=0;i<v.size();i++) v[i]=rand() % 1000000;
    //for (int i=0;i<v.size();i++) v[i]=i;;

    for (int i=0; i<5; i++) printf("%d\n",v[i]);
    v.merge_sort();
    for (int i=0; i<5; i++) printf("%d\n",v[i]);
    printf("is_sorted = %d\n", v.is_sorted());
}

void test_insertion_sort() {
    const int N = 1000;
    vec<int> v(N);
    srand(7);
    for (int i=0;i<v.size();i++) v[i]=rand() % 1000000;
    //for (int i=0;i<v.size();i++) v[i]=i;

    for (int i=0; i<5; i++) printf("%d\n",v[i]);
    v.insertion_sort();
    for (int i=0; i<5; i++) printf("%d\n",v[i]);
    printf("is_sorted = %d\n", v.is_sorted());
}

void test_search() {
    const int N = 5;
    vec<int> v(N);
    for (int i=0;i<N;i++) v[i]=i*2;
    int x=5;
    try {
        v.search(x);
    }
    catch (const char *err) {
        puts (err);
    }
}

class yyy {
    public:
    int x;
    yyy& update(int ix) {
        x=ix; return *this;
    }
};

void testoper() {
    int i,n=5;
    vec<yyy> v1,v2,v3;
    yyy y;
    for (i=0;i<n;i++) {
        v1.push(y.update(i*2));
    }
    v2=v1;
    for (i=0;i<v2.size();i++) printf("%d\n",v2[i].x);
    if (!(v2==v1));
}

int main() {
    //sub2();
    testoper();
    return 0;
}
