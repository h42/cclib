#ifndef _JFILE_H
#define _JFILE_H
#ifndef _JSTRING_H
#include "jlib/jstring.h"
#endif
#include <fcntl.h>
class jfile {
    private:
	long filesize;
	char *buf,*buf2;
	int  fd,size,size2,p1,p2;
    public:
	jfile(const char *fn=0,char *mode=0);
	~jfile(void) {close();};
	int open(char *fn,int flags=O_RDONLY,int mode=0664);
	int close();
	int gets(jstring &);
	int puts(jstring &);
	int printf(char *,...);
	int read(char *,int);
	int read(jstring);
	int read();
	long seek(long, int=0);
	int write(char *, int);
	int write(jstring);
};
#endif
