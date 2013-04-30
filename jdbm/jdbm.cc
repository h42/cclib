#include <gdbm.h>
#include <malloc.h>
//ccinclude
#ifndef _STR_H
#define _STR_H
#ifndef _GDBM_H
#include <gdbm.h>
#endif

typedef unsigned char uchar;
typedef const char cchar;

class jdbm {
public:
    ~jdbm() {close();};
    int open(cchar *fn, cchar *cflags, int mode);
    void close();
    int insert(void *key, int klen, void *value, int vlen);
    int update(void *key, int klen, void *value, int vlen);
    void *get(void *key, int klen);

    int zrc;
    datum zdatum;
private:
    GDBM_FILE zdb;
};

#endif
//ccinclude

void * jdbm::get(void *key, int klen) {
    datum k;
    k.dptr=(char *)key;
    k.dsize=klen;
    if (zdatum.dptr) free(zdatum.dptr);
    zdatum=gdbm_fetch (zdb, k);
    return zdatum.dptr;
}

int jdbm::update(void *key, int klen, void *value, int vlen) {
    datum k,v;
    k.dptr=(char *)key;
    k.dsize=klen;
    v.dptr=(char *) value;
    v.dsize=vlen;
    zrc=gdbm_store (zdb, k, v, GDBM_REPLACE);
    return zrc ? -1 : 0;
}

int jdbm::insert(void *key, int klen, void *value, int vlen) {
    datum k,v;
    k.dptr=(char *)key;
    k.dsize=klen;
    v.dptr=(char *) value;
    v.dsize=vlen;
    zrc=gdbm_store (zdb, k, v, GDBM_INSERT);
    return zrc ? -1 : 0;
}

void jdbm::close() {
    if (zdb) {
        if (zdatum.dptr) {
            free(zdatum.dptr);
            zdatum.dptr=0;
        }
        gdbm_close (zdb);
        zdb=0;
    }
}

int jdbm::open(cchar *fn, cchar *cflags, int mode=640) {
    int flags;
    if (cflags[0]=='r') flags=GDBM_READER;
    else if (cflags[0]=='w') flags=GDBM_WRITER;
    else if (cflags[0]=='n') flags=GDBM_NEWDB;  //always new
    else if (cflags[0]=='c') flags=GDBM_WRCREAT;  //creat if necessary
    if (cflags[1]=='s') flags |= GDBM_SYNC;
    zdatum.dptr=0;
    zdb = gdbm_open(fn, 1024, flags, mode, 0);
    return zdb ? 0 : -1;
}

