#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>
#include <stdarg.h>

//ccinclude
#ifndef _JFILE_H
#define _JFILE_H

#ifndef _STR_H
#include "my/str.h"
#endif

#include <fcntl.h>
class jfile {
    private:
	long filesize;
	char *buf,*buf2;
	int  fd,size,size2,p1,p2;
    public:
        jfile(const char *fn=0,const char *mode=0);
	~jfile(void) {close();};
        int open(const char *fn,int flags=O_RDONLY,int mode=0664);
        int close();
        const char *getFile();
	int gets(str &);
	int puts(str &);
	int puts(const char *s);
	int printf(char *,...);
	int read(char *,int);
	int read(str);
	int read();
	long seek(long, int=0);
	int write(char *, int);
	int write(str);
};
#endif
//ccinclude

static int jgrow(void **buf,int *size) {
    if (!*buf) *buf=malloc(*size);
    else *buf=realloc(*buf,*size);
    if (!*buf) {
	*size=0;
	return -1;
    }
    return 0;
}

jfile::jfile(const char *ifn, const char *mode) {
    char *fn = (char *)ifn;
    fd=-1;
    size=size2=0;
    p1=p2=-1;
    buf=buf2=0;
    if (!fn) return;
    if (!mode || mode[0]=='r') open(fn,O_RDONLY);
    else if (mode[0]=='w') open(fn,O_WRONLY|O_CREAT|O_TRUNC,0660);
    else if (mode[0]=='a') open(fn,O_WRONLY|O_CREAT|O_APPEND,0660);
}

//
// OPEN / CLOSE
//
int jfile::open(const char *fn,int flags,int mode) {
    if (fd>=0) close();
    fd=::open(fn,flags,mode);
    p1=p2=-1;
    return fd;
}

int jfile::close() {
    if (fd>=0) {
	::close(fd);
	fd=-1;
    }
    if (buf) free(buf);
    return 0;
}

//
// PUTS / GETS
//
const char *jfile::getFile() {
    struct stat st;
    int rc = fstat(fd,&st);
    if (rc<0) return 0;
    size = st.st_size;
    jgrow((void **)&buf,&size);
    if (!buf) return 0;
    rc=read(buf,size);
    return (rc==size) ? buf : 0;
}

int jfile::puts(str &s) {
    int l=s.length(),rc;
    char nl=10;
    if (fd<0) return -1;
    if (l>0) if ((rc=::write(fd,s.cstr(),l))) return rc;
    return ::write(fd,&nl,1);
}

int jfile::puts(const char *s) {
    int l=strlen(s),rc;
    char nl=10;
    if (fd<0) return -1;
    if (l>0) if ((rc=::write(fd,s,l))) return rc;
    return ::write(fd,&nl,1);
}

int jfile::gets(str &s) {
    int c,sp1=0,cr=0;
    if (fd<0) return -1;
    size=size2=512;
    if (!buf) ::jgrow((void **)&buf,&size);
    if (!buf2) jgrow((void **)&buf2,&size2);
    if (!buf || !buf2) return -1;
    if (p2==0) return 0;
    while (1) {
	if (p1>=p2) {
	    p2=::read(fd,buf,size);
	    if (p2<=0) break;
	    p1=0;
	}
	c=buf[p1++];
	if (c==13) {
	    cr=1;
	    continue;
	}
	else if (c==10 || cr) {
	    if (c!=10) p1--;
	    buf2[sp1]=0;
	    s=buf2;
	    return 1;
	}
	else if (sp1<size2-1) buf2[sp1++]=c;
    }
    buf2[sp1]=0;
    s=buf2;
    int frc=1;
    if (p2<0) frc=-1;
    else if (!sp1) frc=0;
    return frc;
}

int jfile::printf(char *fmt,...) {
    if (fd<0) return -1;
    size2=512;
    if (!buf2) jgrow((void **)&buf2,&size2);
    if (!buf2) return -1;
    va_list ap;
    va_start(ap,fmt);
    int rc=vsnprintf(buf2,size2,fmt,ap);
    va_end(ap);
    write(buf2,rc);
    return rc;
}

//
// READ / WRITE
//
int jfile::read() {

    return 0;
}

inline int jfile::read(char *ibuf,int ilen) {
    return (fd>=0) ? ::read(fd,ibuf,ilen) : -1;
}

inline int jfile::read(str s) {
    return (fd>=0) ? ::read(fd,s.cstr(),s.length()) : -1;
}

inline int jfile::write(str s) {
    return (fd>=0) ? ::write(fd,s.cstr(),s.length()) : -1;
}

inline int jfile::write(char *obuf, int olen) {
    return (fd>=0) ? ::write(fd,obuf,olen) : -1;
}

inline long jfile::seek(long offset, int whence) {
    return lseek(fd, offset, whence);
}
