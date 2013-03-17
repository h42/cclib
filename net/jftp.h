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
