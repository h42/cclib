#ifndef _JVEC_H
#define _JVEC_H
template <class T>
class jvec {
public:
    jvec<T>();
    jvec<T>(int n);
    ~jvec() {delete [] buf;}
    void grow(int x);
    T& get(int x);
    void put(T &v,int x);
    T& operator[](int x);
    int size() {return zsize;};
private:
    T *buf;
    int zsize;
};
#endif
