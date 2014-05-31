#include <malloc.h>

//ccinclude
#ifndef _JDBM_H
#define _JDBM_H

#ifndef _GDBM_H
#include <gdbm.h>
#endif

typedef unsigned char uchar;
typedef const char cchar;

#define safefree(d) {if (d.dptr) {free(d.dptr); d.dptr=0;}}

class jdbm {
public:
    ~jdbm() {close();};
    int    open(cchar *fn, cchar *cflags, int mode);
    void   close();
    int    insert(void *key, int klen, void *value, int vlen);
    int    update(void *key, int klen, void *value, int vlen);
    void * get();
    void * get(void *key, int klen);
    int    get_errno() {return gdbm_errno;}
    int    remove(void *key, int klen);
    void * first();
    void * next();
    int    reorg();
    void   sync();

private:
    GDBM_FILE zdb;
    datum zkey,zvalue;
    int zerrno;
};

#endif
//ccinclude

void jdbm::sync() {
    gdbm_sync(zdb);
}

int jdbm::reorg() {
    return gdbm_reorganize(zdb);
}

void * jdbm::first() {
    safefree(zkey);
    zkey = gdbm_firstkey(zdb);
    if (!zkey.dptr) return 0;
    return get();
}

void * jdbm::next() {
    void * d = zkey.dptr;
    if (!d) return 0;
    zkey = gdbm_nextkey(zdb,zkey);
    free(d);
    if (!zkey.dptr) return 0;
    return get();
}

int jdbm::remove(void *key, int klen) {
    datum k;
    k.dptr=(char *)key;
    k.dsize=klen;
    return gdbm_delete (zdb, k);
}

void * jdbm::get() {
    return get(zkey.dptr,zkey.dsize);
}

void * jdbm::get(void *key, int klen) {
    datum k;
    k.dptr=(char *)key;
    k.dsize=klen;
    safefree(zvalue);
    zvalue=gdbm_fetch (zdb, k);
    return zvalue.dptr;
}

int jdbm::update(void *key, int klen, void *value, int vlen) {
    datum k,v;
    k.dptr=(char *)key;
    k.dsize=klen;
    v.dptr=(char *) value;
    v.dsize=vlen;
    return gdbm_store (zdb, k, v, GDBM_REPLACE);
}

int jdbm::insert(void *key, int klen, void *value, int vlen) {
    datum k,v;
    k.dptr=(char *)key;
    k.dsize=klen;
    v.dptr=(char *) value;
    v.dsize=vlen;
    return gdbm_store (zdb, k, v, GDBM_INSERT); //+1 instead of -1 for existing record
}

void jdbm::close() {
    if (zdb) {
        safefree(zkey);
        safefree(zvalue);
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
    zkey.dptr=zvalue.dptr=0;
    zdb = gdbm_open(fn, 1024, flags, mode, 0);
    return zdb ? 0 : -1;
}

