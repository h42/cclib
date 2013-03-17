#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

class demon {
public:
    demon() {puts("DEMON NAME REQUIRED"); exit(8); }
    demon(const char *name, const char *rundir=".", const char *lockdir=".");
    void run();
private:
    char zname[256];
    char zlockdir[256];
    char zrundir[256];
    pid_t zpid, zsid;
};

demon::demon(const char *name, const char *rundir, const char *lockdir) {
    strncpy(zname,name,sizeof(zname)-1);
    strncpy(zrundir,rundir,sizeof(zrundir)-1);
    strncpy(zlockdir,lockdir,sizeof(zlockdir)-1);
}

void demon_lock(const char *name, const char *dir=".") {
    FILE *fh;
    char pname[512];
    snprintf(pname,sizeof(pname),"%s/%s",dir,name);
    fh=fopen(pname,"w");
    if (!fh) {
	printf("UNABLE TO OPEN LOCK FILE\n");
	exit(8);
    }
    fprintf(fh,"%d",getpid());
    fclose(fh);
}

void demon::run() {
    uid_t uid;
    if ((uid=getuid()) != 0) {
	printf("You must be root to start deamon - uid=%d\n",uid);
	exit(8);
    }

    // FORK
    zpid = fork();
    if (zpid < 0) {
	exit(8);
    }
    if (zpid > 0) {
	puts("Parent out");
	exit(0);
    }
    zpid=getpid();
    printf("Daemon pid - %d\n",zpid);
    fflush(stdout);

    // UMASK
    umask(0);

    // OPEN LOGS

    // NEW SESSION ID
    zsid = setsid();
    if (zsid < 0) {
	exit(8);
    }
    // Change the current working directory
    if ((chdir("/")) < 0) {
	// Log any failure here
	exit(8);
    }

    // CLOSE FD'S
    close(1);
    close(2);
    close(0);
}

int main() {
    printf("pid=%d\n",getpid());
    demon d("mydemon");
    d.run();
    printf("Daemon out - %d",getpid());
}
