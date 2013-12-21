#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsock.h"

char buf[256],dt[80],tm[80],m[3],d[3],yr[3],hr[3],minute[3],sec[3];

void mkts(char *dst,char *src,int pos,int len=2) {
    int i;
    for (i=0;i<len;i++) dst[i]=src[i+pos];
    dst[i]=0;
}

int main() {
    jsock js;
    js.open_client("time.nist.gov",13);
    int rc=js.read(buf,sizeof(buf));
    if (rc<=0) {
        fprintf(stderr,"read failed\n");
        return 8;
    }
    puts(buf);
    // 56361 13-03-10 02:47:11 51 0 0 806.2 UTC(NIST) *
    mkts(dt,buf,7,8);
    mkts(tm,buf,16,8);
    mkts(yr,dt,0);
    mkts(m,dt,3);
    mkts(d,dt,6);
    mkts(hr,tm,0);
    mkts(minute,tm,3);
    mkts(sec,tm,6);
    //printf("%s %s %s - %s %s %s\n",yr,m,d,hr,minute,sec);
    puts(buf);
    sprintf(buf,"date --utc %s%s%s%s%s.%s",m,d,hr,minute,yr,sec);
    puts(buf);
    system(buf);
    system("hwclock --systohc --utc");
    system("hwclock --show");
}
