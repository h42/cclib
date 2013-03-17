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
    int   timeout(int x);
    int   write(char *buf,int len);
private:
    int zsd,zasd;
    int zserver;
    int ztimeout;
    struct addrinfo *zai;
};
#endif
