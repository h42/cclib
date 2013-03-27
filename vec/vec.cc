#include <stdio.h>

//ccinclude
#ifndef _VEC_H
#define _VEC_H

#include <stdlib.h>
#include <string.h>
#include <new>

template <class T>
class vec {
public:
    vec<T>();
    vec<T>(int n);
    ~vec();
    void grow(int x=0);
    T& get(int x);
    void put(T &v,int x);
    T& operator[](int x);
    int size() {return zsize;};
    // STACK STUF
    void push(T &x);
    T & pop();
    int sp();
    void sp(T);
    // ALGORITHMS
    void reverse(int n=0);
    void msort(T *b=0, int l=0, int r=-1);
private:
    T *zv;
    int zsize,zsp;
};
#endif
//ccinclude

template <class T>
vec<T>::~vec() {
    if (zv) {
	for (int i=0;i<zsize;i++) (zv+i)->~T();
	free(zv);
    }
}

template <class T>
vec<T>::vec() {
    zsp=0;
    zsize=0;
    zv=0;
}

template <class T>
vec<T>::vec(int x) {
    zsize=x;
    zsp=0;
    zv = (T *) malloc(x*sizeof(T));
    for (int i=0;i<x;i++) new(zv+i) T();
}

template <class T>
void vec<T>::grow(int x) {
    if (x<=0) {
	if (zsize<=0) x=2;
	else x=zsize*2;
    }
    if (zsize>=x) return;

    T* buf2;
    if (zv) buf2 = (T *)realloc(zv,x*sizeof(T));
    else buf2 = (T *)malloc(x*sizeof(T));

    zv=buf2;
    for (int i=zsize;i<x;i++) new(zv+i) T();
    zsize=x;
}

template <class T>
T& vec<T>::get(int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    return zv[x];
}

template <class T>
T& vec<T>::operator[](int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    return zv[x];
}

template <class T>
void vec<T>::put(T &v,int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    zv[x]=v;
}

//
// STACK
//
template <class T>
int vec<T>::sp() {
    return zsp;
}

template <class T>
void vec<T>::sp(T x) {
    zsp=x;
}

template <class T>
void vec<T>::push(T &x) {
    if (zsp > zsize) grow();
    zv[zsp++]=x;
}

template <class T>
T &vec<T>::pop() {
    --zsp;
    if (zsp<0) throw "Stack underflow";
    return zv[--zsp];
}

//
// ALGORITHMS
//
template <class T>
void vec<T>::reverse(int n) {
    n=(n<=0) ? zsize : n;
    int n2 = n /2;
    printf("n=%d n2=%d\n",n,n2);
    T t;
    for (int i=0;i<n2;i++) {
	t=zv[i];
	zv[i]=zv[n-1-i];
	zv[n-1-i]=t;
	printf("i=%d v[%d]=%d %p\n",i,i,zv[i],&zv[i]);
    }
}

template <class T>
void vec<T>::msort(T *b, int l, int r) {
    int i,j,k,m;
    if (r<0) r=zsize-1;
    if (!b) b=(T *)malloc(zsize*sizeof(T));
    if (r<=l) return;
    m = (r + l) / 2;
    msort(b, l, m);
    msort(b, m+1, r);

    // time =0.04
    memcpy(&b[l],&zv[l],(m-l+1)*sizeof(T));
    memcpy(&b[m+1],&zv[m+1],(r-m)*sizeof(T));
    k=i=l;
    j=m+1;
    while (k<=r) {
	if (i>m) memcpy(&zv[k++], &b[j++], sizeof(T));
	else if (j>r) memcpy(&zv[k++], &b[i++], sizeof(T));
	else if (b[i]<b[j]) memcpy(&zv[k++], &b[i++], sizeof(T));
	else memcpy(&zv[k++], &b[j++], sizeof(T));
    }
}
