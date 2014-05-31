#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>


//ccinclude
#ifndef _JPOPEN_H
#define _JPOPEN_H
#endif

class jpopen {
public:
    jpopen() {zfd[0]=zfd[1]=-1; zpid=0;}
    ~jpopen() {close();}
    int *open(const char *sx,int flag);
    void close0();
    void close1();
    int close();
    int system(const char *cmd);
private:
    int zfd[2];
    int zpid;
};
//ccinclude

#define ARGC 32
static int fix_path_x(char *s1,char *s2,char *s3) {
    int i,p1,l,l2,state;
    char *cp,c;
    l=strlen(s1);
    for (i=0,p1=0,state=0;i<l;i++) {
        c=s1[i];
        if (state==0) {
            if (c==' ') continue;
            else if (c==0) {
                s1[0]=0;
                break;
            }
            else {
                s1[p1++]=c;
                state=1;
            }
        }
        else if (state==1) {
            s1[p1++]=c;
            if (!c) break;
        }
    }
    cp=realpath(s1,s3);
    if (!cp) return -1;
    l=strlen(s3);
    l2=strlen(s2);
    if (l+l2>254) return -1;
    if (l && s3[l-1] != '/') s3[l++]='/';
    strcpy(&s3[l],s2);
    return access(s3,X_OK);
}

static int fix_path(char *fn,char *ofn,int len) {
    int i,l,rc,p1=0;
    char *cp,c,sx[256];
    if (fn[0]=='/') {
        strcpy(ofn,fn);
        return 0;
    }
    cp=getenv("PATH");
    l=strlen(cp);
    if (!l) return -1;
    for (i=0;i<l;i++) {
        c=cp[i];
        if (c==':') {
            sx[p1]=0;
            rc=fix_path_x(sx,fn,ofn);
            if (!rc) return 0;
            p1=0;
        }
        else if (p1<255) sx[p1++]=c;
    }
    if (p1) {
        sx[p1]=0;
        rc=fix_path_x(sx,fn,ofn);
        if (!rc) return 0;
    }
    return -1;
}

//
// safe popen - NOT thread safe
//

void jpopen::close0() {
    if (zfd[0]>=0) {
        ::close(zfd[0]);
        zfd[0]=-1;
        zpid=0;
    }
}

void jpopen::close1() {
    if (zfd[1]>=0) {
        ::close(zfd[1]);
        zfd[1]=-1;
        zpid=0;
    }
}

int jpopen::close() {
    int rc=-1,status=-1;
    if (zfd[0]>=0) ::close(zfd[0]);
    if (zfd[1]>=0) ::close(zfd[1]);
    if (zpid>0) {
        waitpid(zpid,&status,0);
        rc =  WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
    zfd[1]=zfd[0]=-1;
    zpid=0;
    return rc;
}

int *jpopen::open(const char *sx,int flag) {
    //
    // flag 1=read zfd[0]   2=write zfd[1]   4==stderr   3,7=read/write   0=jsystem
    // use close1 to terminate forked input may sometimes be required
    //
    int fdout[2],fdin[2]; // from parent perspective
    int argc=0,state=0,i,p1=0;
    char *argv[ARGC],c,path[256],buf[512];

    zfd[0]=-1;
    zfd[1]=-1;
    zpid=0;

    for (i=0;i<512;i++) {
        c=sx[i];
        if (state==0) {
            if (!c) break;
            if (c==' ') continue;
            argv[argc++]=&buf[p1];
            if (argc>=ARGC) return 0;
            if (p1<511) buf[p1++]=c;
            state=1;
        }
        else if (state==1) {
            if (!c) {
                buf[p1++]=0;
                break;
            }
            if (c==' ') {
                buf[p1++]=0;
                state=0;
            }
            else buf[p1++]=c;
        }
    }
    buf[p1]=0;
    if (!argc) return 0;
    argv[argc]=0;
    strncpy(path,argv[0],sizeof(path)-1);path[sizeof(path)-1]=0;
    if (fix_path(argv[0],path,sizeof(path))) return 0;

    if (flag&1) {
        if (pipe(fdin)) return 0;
    }

    if (flag&2) {
        if (pipe(fdout)) {
            if (flag&1) {
                ::close(fdin[0]);
                ::close(fdin[1]);
            }
            return 0;
        }
    }

    zpid=fork();
    // fork error
    if (zpid<0) {
        if (flag&1) {
            ::close(fdin[0]);
            ::close(fdin[1]);
        }
        if (flag&2) {
            ::close(fdout[0]);
            ::close(fdout[1]);
        }
        return 0;
    }

    // child process
    if (!zpid) {
        if (flag&2) {
            dup2(fdout[0],0);
            ::close(fdout[0]);
            ::close(fdout[1]);
        }
        if (flag&1) {
            dup2(fdin[1],1);
            if (flag&4) dup2(fdin[1],2); // stderr ???
            ::close(fdin[0]);
            ::close(fdin[1]);
        }
        execv(path,argv);
        exit(127);
    }

    // parent process
    if (flag&1) {
        ::close(fdin[1]);
        zfd[0]=fdin[0];
    }
    if (flag&2) {
        ::close(fdout[0]);
        zfd[1]=fdout[1];
    }
    return zfd;
}

int jpopen::system(const char *cmd) {
    int *pfd;
    pfd=open(cmd,0);
    return (!pfd) ? -1 : close();
}
