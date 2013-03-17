#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "jsock.h"

int main() {
	int rc,sd;
	char buf[80]="hey";
	jsock js;
	sd = js.open_client("127.0.0.1",4242);
	printf("open sd=%d\n",sd);
	if (sd<0) {
		return -1;
	}
	//sleep(4);
	rc=js.write(buf,strlen(buf)+1);
	fsync(js.sd());
	printf("write rc=%d\n",rc);

	strcpy(buf,"bad read");
	rc=js.read(buf,sizeof(buf));
	printf("read rc=%d %s\n",rc,buf);
	return 0;
}
