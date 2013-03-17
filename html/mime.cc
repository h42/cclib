#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

class xxx {
public:
    int getfile(const char *fn);
    int getline();
    int process();
    char zbuf[100000],lbuf[1000],boundary[256];
    int recl;
    int p1;
};

int xxx::getfile(const char *fn) {
    int fd = open(fn,O_RDONLY);
    if (fd<0) return -1;
    recl = read(fd,zbuf,sizeof(zbuf));
    return recl;
}

int xxx::getline() {
    int c,i,j=0;
    for (i=p1; i<recl; i++) {
	c=zbuf[i];
	if (c==10) break;
	else lbuf[j++]=c;
    }
    lbuf[j]=0;
    if (j && lbuf[j-1]==13) lbuf[j-1]=0;
    p1=i+1;
    //printf("j=%d p1=%d lb=%s\n",j,p1,lbuf);
    return 0;
}

int xxx::process() {
    int state=0,c;
    p1=0;
    getline();
    strcpy(boundary,lbuf);
    for (int i=0; ;i++) {
	getline();
	if (memcmp(lbuf,boundary,strlen(boundary)) == 0) {
	    puts(lbuf);
	    if (strcmp(lbuf,boundary)) break;
	}
    }
    return 0;
}

int main() {
    xxx x1;
    int rc;
    rc = x1.getfile("mimedata");
    if (rc<1) return 8;
    x1.process();
    printf("flen=%d\nbnd=+++%s+++\n",x1.recl,x1.boundary);

    return 0;
}
