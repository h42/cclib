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

int main() {
	int rc;
	remove("test.log");
	jsock js;
	rc=js.run(4242,myfunc);
	return 0;
}
