
//ccinclude
#ifndef _TUP_H
#define _TUP_H

template <class T, class T2>
class tup {
public:
    tup() {}
    tup(T t, T2 t2) : zt(t) ,zt2(t2) {}
    T &fst();
    T2 &snd();
private:
    T zt;
    T2 zt2;
};

template <class T,class T2>
T &tup<T,T2>::fst() {
    return zt;
}

template <class T,class T2>
T2 &tup<T,T2>::snd() {
    return zt2;
}

#endif
//ccinclude
