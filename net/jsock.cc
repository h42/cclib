#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <poll.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

//ccinclude
#ifndef _JSOCK_H
#define _JSOCK_H
#define JSOCK_POLLERR -7
#define JSOCK_TIMEOUT -99

void  gotsig(int sig);

class jsock {
public:
    jsock();
    ~jsock();
    int   accept();
    int   addrinfo(const char *host,unsigned short iport);
    void  close();
    int   listen();
    int   open_server(short port);
    int   open_client(const char *host,unsigned short port);
    int   read(char *buf,int len);
    int   run(int port,int (*callback)(jsock&));
    int   sd(int x=-1);
    void  sync();
    int   timeout(int x);
    int   write(const char *buf,int len);
private:
    int zsd,zasd;
    int zserver;
    int ztimeout;
    struct addrinfo *zai;
};
#endif
//ccinclude

static int alldone;

int jsock::sd(int x) {
    if (x>=0) zsd=x;
    else x=zsd;
    return x;
}

void  jsock::sync() {
    if (zsd<0) return;
    syncfs(zsd);
}

jsock::jsock() {
    zsd=-1;
    zserver=0;
    ztimeout=10000;
    zai=0;
}

jsock::~jsock() {
    if (zsd!=-1) {
	close();
    }
    if (zai) freeaddrinfo(zai);
}

void jsock::close() {
    if (zsd!=-1) {
	::close(zsd);
    }
}

int jsock::timeout(int x) {
    if (x>=0) ztimeout=x;
    return ztimeout;
}

//////////////////////////////
//      SERVER
//////////////////////////////
int jsock::open_server(short port) {
    int rc=0;
    struct sockaddr_in name;
    zsd=socket(AF_INET,SOCK_STREAM,0);
    name.sin_family=AF_INET;
    name.sin_port  =htons(port);
    name.sin_addr.s_addr=INADDR_ANY;
    int sopt=1;
    setsockopt(zsd,SOL_SOCKET,SO_REUSEADDR,&sopt,sizeof(sopt));
    if (bind(zsd,(struct sockaddr *)&name,sizeof(name))<0) {
	close();
	rc=zsd= -1;
    }
    return rc;
}

int jsock::listen() {
    int rc=::listen(zsd,5);
    return (rc)?-1:0;
}

int jsock::accept() {
    zasd = ::accept(zsd,0,0); //null parm can be used to get client address
    return zasd;
}

void gotsig(int sig) {
    int rc,status;
    //printf("signal=%d\n",sig);
    if (sig==SIGCHLD) {
	while (1) {
	    rc=waitpid(-1,&status,WNOHANG);
	    //printf("sig child rc=%d\n",rc);
	    if (rc<=0) return;
	}
    }
    alldone=1;
}

int jsock::run(int port,int (*callback)(jsock&)) {
    int rc,sd,hit;
    struct pollfd pfd;
    alldone=0;
    signal(SIGCHLD,gotsig); // 17
    signal(SIGINT,gotsig);  //ctrl c - 2
    signal(SIGQUIT,gotsig); //ctrl \ - 3
    signal(SIGTERM,gotsig); //kill - 15
    if (open_server(port)<0) return -1;
    if (listen()<0) return -1;
    while (!alldone) {
	pfd.fd=zsd;
	pfd.events=POLLIN;
	pfd.revents=0;
	hit=0;
	rc=poll(&pfd,1,1000);
	if (rc>0) {
	    if (pfd.revents&POLLIN) hit=1;
	    else rc=JSOCK_POLLERR;
	}
	else rc=JSOCK_TIMEOUT;
	if (!hit) continue;
	if ( (sd = accept()) < 0) {
	    return -1;
	}
	//printf("forking rc=%d\n",rc);
	rc=fork();
	if (rc<0) {
	    //send msg to sd ??
	    break;
	}
	if (rc==0) {
            ::close(zsd); //listen socket
	    zsd = sd;
	    rc=(callback)(*this);
	    exit(rc);
	}
	//parent
	::close(sd); // close accept sd
    }
    return 0;
}

//////////////////
// Client
//////////////////
int jsock::read(char *buf,int len) {
    int rc,etime,p1=0,exact=0;
    struct timeval tv0,tv1;
    if (len==0) return -1;
    if (len<0) {
	exact=1;
	len=-len;
    }
    gettimeofday(&tv0,0);
    while (1) {
	gettimeofday(&tv1,0);
	etime=(tv1.tv_sec-tv0.tv_sec)*1000 + (tv1.tv_usec-tv0.tv_usec)/1000;
	if (ztimeout>0 && ztimeout<=etime) break; //return timeout
	struct pollfd pfd;
	pfd.fd=zsd;
	pfd.events=POLLIN|POLLPRI;
	pfd.revents=0;
	rc=poll(&pfd,1,ztimeout-etime);
	if (rc>0) {
	    if (pfd.revents&POLLIN) {
		rc=::read(zsd,&buf[p1],len-p1);
		if (rc<=0) return rc;
		p1+=rc;
		if (exact==0 || p1>=len) return p1;
	    }
	    else rc=JSOCK_POLLERR;
	}
    }
    return JSOCK_TIMEOUT;
}

int jsock::write(const char *buf,int len) {
    int rc,p1=0,etime;
    struct timeval tv0,tv1;
    struct pollfd pfd;
    if (len<=0) len=strlen(buf)+1;
    gettimeofday(&tv0,0);
    while (1) {
	gettimeofday(&tv1,0);
	etime=(tv1.tv_sec-tv0.tv_sec)*1000 + (tv1.tv_usec-tv0.tv_usec)/1000;
	if (ztimeout>0 && ztimeout<=etime) break; //return timeout
	pfd.fd=zsd;
	pfd.events=POLLOUT;
	pfd.revents=0;
	rc=poll(&pfd,1,ztimeout-etime);
	if (rc>0) {
	    if (pfd.revents&POLLOUT) {
		rc=::write(zsd,&buf[p1],len);
		if (rc<0) {
		    if (errno==EINTR);
		    else return -1;
		}
		if ((p1 += rc)>=len) return p1;
	    }
	    else return JSOCK_POLLERR;
	}
	else return JSOCK_TIMEOUT;
    }
    return 0;
}

int jsock::addrinfo(const char *host,unsigned short iport) {
    struct addrinfo hints;
    char port[40];

    if (zai) freeaddrinfo(zai);

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    //hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_family = AF_INET;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    snprintf(port,sizeof(port),"%u",iport);
    //fprintf(stderr,"host=%s port=%s\n",host,port);

    return getaddrinfo(host,port , &hints, &zai);
}

int jsock::open_client(const char *host,unsigned short port) {
    int rc;
    if (zsd>=0) close();
    zsd=socket(AF_INET,SOCK_STREAM,0);
    if (zsd<0) {
	return -2;
    }
    rc=addrinfo(host,port);
    if (rc) return -3;

    if (connect(zsd,zai->ai_addr,sizeof(struct sockaddr))<0) {
	close();
	return -4;
    }
    return zsd;
}

//////////////////
// Subs
//////////////////
/////////////////////////////////
/*
int getport(int sd) {
    int port;
    socklen_t namelen;
    struct sockaddr_in name;
    namelen=sizeof(name);
    getsockname(sd,(struct sockaddr *)&name,&namelen);
    port=ntohs(name.sin_port);
    //printf("port=%d\n",port);
    return port;
}

int getsockaddr(int sd) {
    int addr;
    socklen_t namelen;
    struct sockaddr_in name;
    namelen=sizeof(name);
    getsockname(sd,(struct sockaddr *)&name,&namelen);
    addr=ntohl(name.sin_addr.s_addr);
    //printf("port=%d\n",port);
    return addr;
}
*/

