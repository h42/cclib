#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "jpopen.h"

int main() {
    puts("hey");
    jpopen p;
    p.system("date");
    char buf[256];
    fgets(buf,sizeof(buf),stdin);
    puts(buf);

    int *fd = p.open("cat",3);
    write(fd[1],buf,strlen(buf));
    buf[0]=0;
    fsync(fd[1]);
    read(fd[0],buf,sizeof(buf));
    puts(buf);
    return 0;
}


