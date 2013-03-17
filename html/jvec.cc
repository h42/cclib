#include <stdio.h>
#include <stdlib.h>

//ccinclude
#ifndef _JVEC_H
#define _JVEC_H
template <class T>
class jvec {
public:
    jvec<T>();
    jvec<T>(int n);
    ~jvec() {delete []zbuf;}
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
int jvec<T>::sp() {
    return zsp;
}

template <class T>
void jvec<T>::sp(T x) {
    zsp=x;
}

template <class T>
void jvec<T>::push(T &x) {
    if (zsp > zsize) grow();
    zbuf[zsp++]=x;
}

template <class T>
T &jvec<T>::pop() {
    --zsp;
    if (zsp<0) throw "Stack underflow";
    return zbuf[--zsp];
}

template <class T>
jvec<T>::jvec() {
    zsp=0;
    zsize=0;
    zbuf=0;
}

template <class T>
jvec<T>::jvec(int x) {
    zbuf=new T[x];
    zsize=x;
    zsp=0;
}

template <class T>
void jvec<T>::grow() {
    zsize*=2;
    int n2 = zsize * 2;
    T *buf2 = new T[n2];
    for (int i=0;i<zsize;i++) buf2[i]=zbuf[i];
    delete []zbuf;
    zbuf = buf2;
    zsize=n2;
}

template <class T>
void jvec<T>::grow(int x) {
    if (zsize>=x) return;
    if (x<2) x=2;
    T *buf2 = new T(x);
    for (int i=0;i<zsize;i++) buf2[i]=zbuf[i];
    delete []zbuf;
    zbuf = buf2;
    zsize=x;
}

template <class T>
T& jvec<T>::get(int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    return zbuf[x];
}

template <class T>
T& jvec<T>::operator[](int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    return zbuf[x];
}

template <class T>
void jvec<T>::put(T &v,int x) {
    if (x<0 || x>=zsize) throw "subscript out of bounds";
    zbuf[x]=v;
}

