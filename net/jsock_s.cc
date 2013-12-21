#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include "jsock.h"

char buf[4096];

int myfunc(jsock &js) {
	int rc;
	//FILE *fh;
	char buf[80];
	//fh=fopen("test.log","a");
	rc=js.read(buf,9);
	fprintf(stdout,"\nread rc=%d\n%s\n",rc,buf);
	if (rc<=0) return 8;
	rc=js.write(buf,strlen(buf)+1);
	fprintf(stdout,"write rc=%d\n",rc);
	//fclose(fh);
	return 0;
}


int client(jsock &js) {
    const char *msg=
        "HTTP/1.1 200 OK\n"
        "Connection: close\n"
        "Content-type: text/html; charset=utf-8\n\n"

	"<!DOCTYPE HTML>\n"
	"<html>\n"
	"<head>\n"
	"<meta http_equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n"
	"<title>\n"
	"Test Page\n"
	"</title>\n"

	"</head>\n"
	"<body>\n"
	"<h2>Hey Now!</h2>\n"
	"</body>\n"
	"</html>\n"
       ;
    js.read(buf,sizeof(buf)-1);
    js.write(msg, strlen(msg));
    //js.sync();
    return 0;
}

int main() {
	remove("test.log");
	jsock js;
        js.run(8080,client);
	return 0;
}
