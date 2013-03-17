#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include "jsock.h"

//ccinclude
#ifndef _JFTP_H
#define _JFTP_H

class jftp {
public:
	jftp() { zquiet=0; }
	int cd(const char *parm="");
	int chmod(const char *parm);
	int cmd_x(const char *fmt,...);
	int connect(const char *host, const char *user, const char *pw);
	int get(const char *fn, const char *fn2=0);
	int help();
	int pwd();
	int ll(const char *parms="", const char *fn=0);
	int ls(const char *parms="", const char *fn=0);
	int put(const char *fn, const char *fn2=0);
	int quit();
	int rm(const char *parm);
	int zquiet;
private:
	char ibuf[4096],obuf[4096];
	jsock sk,sk2;
	int get_data(const char *fn);
	int cmd_pasv();
	int cmd_stor(const char *fn, const char *fn2);
	int cmd_stor2(int fd, const char *fn);
	int get_resp(int timeout);
	int cmd_user(const char *user, const char *pw);
};

#endif
//ccinclude

static int chk_resp(char *s) {
    int i,c,state=0,resp=-1;
    for (i=0;;i++) {
	c=s[i];
	if (!c) break;
	if (state==0) {
	    if (s[i+3]==' ') {
		if (isdigit(c) && isdigit(s[i+1]) && isdigit(s[i+2])) {
		    resp=(c-'0')*100 + (s[i+1]-'0')*10 + (s[i+2]-'0');
		    return resp;
		}
		state=1;
	    }
	}
	else if (state==1) {
	    if (c==10) state=0;
	}
    }
    return resp;
}

int jftp::get_resp(int timeout) {
	int i,rc,t;
	t=sk.timeout(-1);
	for (i=0;timeout<=0 || i<timeout;i++) {
		sk.timeout(1000);
		rc=sk.read(ibuf,sizeof(ibuf));
		ibuf[ (rc >= 0 && rc < (int)sizeof(ibuf))
			 ? rc : ( rc > 0 ? sizeof(ibuf)-1 : 0 ) ] = 0;
		if (!zquiet) printf("%s",ibuf);
		sk.timeout(t);
		rc=chk_resp(ibuf);
		if (rc>=100) return rc;
	}
	return -1;
}

int jftp::cmd_pasv() {
	unsigned int daddr;
	short dport;
	char zdotaddr[20];
	int rc,resp,state,p1,i,c,sd;
	int num[6]={0,0,0,0,0,0};
	snprintf(obuf,sizeof(obuf),"PASV\r\n");
	rc=sk.write(obuf,strlen(obuf));
	resp=get_resp(10);
	if (resp != 227) {
		if (!zquiet) fprintf(stderr,
			"response %d received from pasv cmd - expecting 227\n",resp);
		return -1;
	}
	for (i=0,state=0,p1=0;i<80;i++) {
		c=ibuf[i];
		if (c==0) break;
		if (state==0) {
			if (c=='(') state=1;
		}
		else if (state==1) {
			if (c==',' || c==')') {
				p1++;
				if (p1>=6) {
					state=2;
					break;
				}
			}
			else if (c>='0' && c<='9') num[p1]=num[p1]*10+c-'0';
		}
	}
	if (state != 2) {
		fprintf(stderr,"unable to parse -  %s\n",ibuf);
		return -1;
	}
	daddr=(num[0]<<24)+(num[1]<<16)+(num[2]<<8)+num[3];
	dport=(num[4]<<8)+num[5];
	sprintf(zdotaddr,"%d.%d.%d.%d",num[0],num[1],num[2],num[3]);
	if ((sd=sk2.open_client(zdotaddr,dport))<0) {
		if (!zquiet) fprintf(stderr,"unable to open passive socket\n");
		return -1;
	}
	if (!zquiet) 
	  fprintf(stderr,"passive socket opened - sd=%d - %s\n",sd,zdotaddr);
	return 0;
}

int jftp::cmd_stor2(int fd, const char *fn) {
	int rc,len;
	snprintf(obuf,sizeof(obuf),"STOR %s\r\n",fn);
	rc=sk.write(obuf,strlen(obuf));
	if (rc<0) {
		if (!zquiet) printf("stor write failed rc=%d\n",rc);
		return -1;
	}
	rc=get_resp(10);
	if (rc==150) {
		while ((len=read(fd,obuf,sizeof(obuf)))>0) {
			rc=sk2.write(obuf,len);
			if (rc!=len) {
				if (!zquiet) fprintf(stderr,"jsock_write failed - rc=%d\n",rc);
				return -1;
			}
		}
	}
	return 0;
}

int jftp::cmd_stor(const char *fn, const char *fn2) {
	int fd,rc;
	if ((fd=open(fn,O_RDONLY)) <0) {
		if (!zquiet) fprintf(stderr,"file %s open failed\n",fn);
		return -1;
	}
	rc=cmd_stor2(fd,fn2);
	close(fd);
	sk2.close();
	rc=get_resp(10);
	return (rc==226) ? 0 : -1;
}

int jftp::cmd_user(const char *user, const char *pw) {
	int rc,resp;
	snprintf(obuf,sizeof(obuf),"user %s\r\n",user);
	rc=sk.write(obuf,strlen(obuf));
	if (rc<0) printf("write rc=%d\n",rc);
	resp=get_resp(10);
	if (resp==230) return 0;
	if (resp !=  331) {
		if (!zquiet) fprintf(stderr,
			"response %d received from user cmd - expecting 331 or 230\n",resp);
		return -1;
	}
	// 331
	if (!pw || !pw[0]) pw=getpass("Enter password ");
	snprintf(obuf,sizeof(obuf),"pass %s\r\n",pw);
	rc=sk.write(obuf,strlen(obuf));
	if (rc<0) printf("write rc=%d\n",rc);
	resp=get_resp(10);
	if (resp != 230) {
		if (!zquiet) fprintf(stderr,
			"response %d received from pass cmd - expecting 230\n",resp);
		return -1;
	}
	return 0;
}

int jftp::cmd_x(const char *fmt,...) {
	int rc,n;
	va_list ap;
	va_start(ap, fmt);
	n = vsnprintf(obuf, sizeof(obuf), fmt, ap);
	va_end(ap);
	int l = strlen(obuf);
	if (l < 2 || memcmp(&obuf[l-2], "\r\n", 2))
		if (l+2 < (int)sizeof(obuf)) strcat(obuf,"\r\n");
	rc=sk.write(obuf,strlen(obuf));
	if (rc<0) {
		printf("cmd_x - %s - write rc=%d\n",obuf,rc);
		return -1;
	}
	ibuf[0]=0;
	rc=get_resp(7);
	return rc;
}

int jftp::get_data(const char *fn) {
	int fd,rc,resp;
	if (fn && fn[0]) {
		fd=open(fn,O_WRONLY|O_TRUNC|O_CREAT,0600);
		if (fd<0) {
			if (!zquiet) fprintf(stderr,"file %s open failed\n",fn);
			return -1;
		}
	}
	else fd=1;
	resp=get_resp(10);
	if (resp==150) {
		while (1) {
			rc=sk2.read(ibuf,sizeof(ibuf));
			if (rc==JSOCK_TIMEOUT) continue;
			if (rc<=0) break;
			write(fd,ibuf,rc);
		}
		rc=get_resp(10);
		rc=(rc==226) ? 0 : -1;
	}
	else rc=-1;
	if (fn && fn[0]) close(fd); // do not check fd != 1
	sk2.close();
	return rc;
}

//
// API
//
int jftp::help() {
	int rc = cmd_x("HELP\r\n");
	return (rc == 214) ? 0 : -1;
}

int jftp::cd(const char *parm) {
	int rc = cmd_x("CWD %s\r\n", parm);
	return (rc == 250) ? 0 : -1;
}

int jftp::chmod(const char *parm) { // ex. parm = "770 FN"
	int rc=-1;
	if (parm) cmd_x("SITE CHMOD %s\r\n", parm);
	return (rc == 200) ? 0 : -1;
}

int jftp::connect(const char *host, const char *user, const char *pw) {
	int resp,sd;
	sd=sk.open_client(host,21);
	if (sd<0) return -1;
	resp=get_resp(10);
	if (resp != 220) {
		if (!zquiet) {
			printf("%s",ibuf);
			printf("resp=%d\n",chk_resp(ibuf));
		}
		return -1;
	}
	if (!zquiet) printf("connect open resp = %d\n",resp);
	return (user && user[0]) ? cmd_user(user, pw) : 0;
}

int jftp::get(const char *fn, const char *fn2) {
	int rc;
	if (!fn2) {
		int p1=0;
		for (int i = 0;; i++) {
			if (!fn[i]) break;
			if (fn[i] == '/') p1 = i+1;
		}
		fn2 = &fn[p1];
	}
	if (!zquiet) fprintf(stderr, "Retrieving %s ==> %s\n",fn, fn2);
	rc = cmd_pasv();
	rc = cmd_x("TYPE I\r\n");
	snprintf(obuf,sizeof(obuf),"RETR %s\r\n", fn);
	rc=sk.write(obuf,strlen(obuf));
	if (rc<0) if (!zquiet) printf("retr write rc=%d\n",rc);
	rc=get_data(fn2);
	return rc;
}

int jftp::ll(const char *parms, const char *fn) {
	int rc;
	rc = cmd_pasv();
	snprintf(obuf,sizeof(obuf),"LIST %s\r\n", parms);
	rc=sk.write(obuf,strlen(obuf));
	if (rc<0) if (!zquiet) printf("list write rc=%d\n",rc);
	rc=get_data(fn);
	return rc;
}

int jftp::ls(const char *parms, const char *fn) {
	int rc;
	rc = cmd_pasv();
	snprintf(obuf,sizeof(obuf),"NLST %s\r\n", parms);
	rc=sk.write(obuf,strlen(obuf));
	if (rc<0) if (!zquiet) printf("list write rc=%d\n",rc);
	rc=get_data(fn);
	return rc;
}

int jftp::put(const char *fn, const char *fn2) {
	int rc;
	if (!fn2) {
		int p1=0;
		for (int i = 0;; i++) {
			if (!fn[i]) break;
			if (fn[i] == '/') p1 = i+1;
		}
		fn2 = &fn[p1];
	}
	if (!zquiet) fprintf(stderr, "Storing %s as %s\n",fn, fn2);
	rc = cmd_pasv();
	rc = cmd_x("TYPE I\r\n");
	return cmd_stor(fn, fn2);
}

int jftp::pwd() {
	int rc = cmd_x("PWD\r\n");
	return (rc == 257) ? 0 : -1;
}

int jftp::quit() {
	int rc = cmd_x("QUIT\r\n");
	return (rc == 221) ? 0 : -1;
}

int jftp::rm(const char *parm) {
	int rc=-1;
	if (parm) rc = cmd_x("DELE %s\r\n",parm);
	return (rc == 250) ? 0 : -1;
}

