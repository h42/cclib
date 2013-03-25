#include <stdio.h>
#include <stdlib.h>
#include <new>

//ccinclude
#ifndef _VEC_H
#define _VEC_H
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
private:
    T *zbuf;
    int zsize,zsp;
};
#endif
//ccinclude

template <class T>
vec<T>::~vec() {
    if (zbuf) {
	for (int i=0;i<zsize;i++) (zbuf+i)->~T();
	free(zbuf);
    }
}

template <class T>
vec<T>::vec() {
    zsp=0;
    zsize=0;
    zbuf=0;
}

template <class T>
vec<T>::vec(int x) {
    zsize=x;
    zsp=0;
    zbuf = (T *) malloc(x*sizeof(T));
    for (int i=0;i<x;i++) new(zbuf+i) T();
}

template <class T>
void vec<T>::grow(int x) {
    if (x<=0) {
	if (zsize<=0) x=2;
	else x=zsize*2;
    }
    if (zsize>=x) return;

    T* buf2;
    if (zbuf) buf2 = (T *)realloc(zbuf,x*sizeof(T));
    else buf2 = (T *)malloc(x*sizeof(T));

    zbuf=buf2;
    for (int i=zsize;i<x;i++) new(zbuf+i) T();
    zsize=x;
}

template <class T>
T& vec<T>::get(int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    return zbuf[x];
}

template <class T>
T& vec<T>::operator[](int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    return zbuf[x];
}

template <class T>
void vec<T>::put(T &v,int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    zbuf[x]=v;
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
    zbuf[zsp++]=x;
}

template <class T>
T &vec<T>::pop() {
    --zsp;
    if (zsp<0) throw "Stack underflow";
    return zbuf[--zsp];
}

