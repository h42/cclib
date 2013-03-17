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
#include "jftp.h"

int main(int argc,char **argv) {
	int rc, bigrc;

	// CONNECT
	jftp ftp1;

	/*
	bigrc = ftp1.connect("jld2.com", 0, 0);
	if (bigrc) return -1;
	ftp1.help();
	ftp1.cmd_x("AUTH TLS\r\n");
	rc = ftp1.quit();
	return 0;
	*/

	bigrc = ftp1.connect("jld2.com", "kbswinge", 0);
	ftp1.zquiet=0; // set to 0 for response - 1 to eliminate responses
	rc = ftp1.ll("");
	rc = ftp1.quit();
	return 0;


	rc = ftp1.help();

	rc= ftp1.cd("arc");
	rc = ftp1.pwd();

	rc= ftp1.cd("..");
	rc = ftp1.pwd();

	ftp1.get("arc/cc.bz2","temp");

	ftp1.rm("temp");

	ftp1.put("temp");

	ftp1.ll("temp"); // get_data sometimes goes slow - so not a problem??
	ftp1.chmod("770 temp");
	ftp1.ll("temp");

	ftp1.zquiet=0; // 0 for upload / download stats - 1 for no noise
	rc = ftp1.quit();

	return 0;
}
