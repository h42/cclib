#include <stdio.h>
#include <stdlib.h>

//ccinclude
#ifndef _VEC_H
#define _VEC_H
template <class T>
class vec {
public:
    vec<T>();
    vec<T>(int n);
    ~vec() {delete []zbuf;}
    void grow();
    void grow(int x);
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

template <class T>
vec<T>::vec() {
    zsp=0;
    zsize=0;
    zbuf=0;
}

template <class T>
vec<T>::vec(int x) {
    zbuf=new T[x];
    zsize=x;
    zsp=0;
}

template <class T>
void vec<T>::grow() {
    zsize*=2;
    int n2 = zsize * 2;
    T *buf2 = new T[n2];
    for (int i=0;i<zsize;i++) buf2[i]=zbuf[i];
    delete []zbuf;
    zbuf = buf2;
    zsize=n2;
}

template <class T>
void vec<T>::grow(int x) {
    if (zsize>=x) return;
    if (x<2) x=2;
    T *buf2 = new T(x);
    for (int i=0;i<zsize;i++) buf2[i]=zbuf[i];
    delete []zbuf;
    zbuf = buf2;
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

