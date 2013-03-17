#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "jvec.h"
#include "jre.h"


//ccinclude
#ifndef _JDIR_H
#define _JDIR_H
char **jdir_ls(char *dir,int flag);
int jdir_mk(char *dir,int mode);
int jdir_rm(char *dir);
int jdir_cp(char *idir,char *idir2);
#endif
//ccinclude

//
// JDIR_LS
//
static int do_file(char *fn,char type,char ***dl) {
	int size=jvsize(*dl),vlen=jvlen(*dl),len,start;
	char *buf;
	if (!memcmp(fn,"./",2)) fn+=2;
	start=(type)?1:0;
	len=strlen(fn)+1+start;
	buf=malloc(len);
	if (!buf) return -1;
	memcpy(&buf[start],fn,len);
	if (type) buf[0]=type;
	if (vlen>=size) {
		size=size<1000?size*2:size+1024;
		if (!(*dl=jvgrow(*dl,size))) return -1;
		if (vlen>=jvsize(*dl)) return -1;
	}
	if (jvpush(*dl,buf)) return -1;
	return 0;
}

int do_dir(char *dir,int flag,char ***dl) {
	int l,l2,rc,subdir=flag&1,dotype=flag&2;
	char fn[256],*cp1,*cp2;
	DIR *d1;
	struct stat st1;
	struct dirent *de1;
	if (!dir || !dir[0]) {
		strcpy(fn,"./");
		l=2;
	}
	else {
		l=strlen(dir);
		memcpy(fn,dir,l);
		if (fn[l-1] != '/') fn[l++]='/';
		fn[l]=0;
	}
	d1=opendir(fn);
	if (!d1) {
		//fprintf(stderr,"opendir failed - %s\n",fn);
		return -1;
	}
	while (1) {
		de1=readdir(d1);
		if (!de1) break;
		cp1=&fn[l];
		cp2=de1->d_name;
		if (cp2[0]=='.' && (!cp2[1] || (cp2[1]=='.' && cp2[2]==0))) continue;
		l2=0;
		while ((*(cp1++)=*(cp2++))) l2++;
		rc=lstat(fn,&st1);
		if (rc) {
			//fprintf(stderr,"lstat failed - %s\n",fn);
			return -1;
		}
		if (S_ISREG(st1.st_mode)) {
			if (do_file(fn,dotype?' ':0,dl)) return -1;
		}
		else if (S_ISDIR(st1.st_mode)) {
			if (subdir) {
				strcat(fn,"/");
				if (do_dir(fn,flag,dl)) return -1;
			}
			else {
				if (do_file(fn,dotype?'D':0,dl)) return -1;
			}
		}
		else if (S_ISLNK(st1.st_mode)) {
			if (do_file(fn,dotype?'L':0,dl)) return -1;
		}
		//else continue;
	}
	closedir(d1);
	return 0;
}

char **jdir_ls(char *dir,int flag) {
	int rc;
	char **dl2=jvnew(64,free);
	char ***dl=&dl2;
	if (!dl2 || !dl) return 0;
	rc=do_dir(dir,flag,dl);
	if (rc) {
		jvfree(*dl);
		return 0;
	}
	jvsort(*dl,jvsort_sz);
	return *dl;
}

//
// JDIR_MK
//
static int jmkdir(char *dir,mode_t mode) {
	mode_t m=umask(0);
	int rc=mkdir(dir,mode);
	umask(m);
	return rc;
}

int jdir_mk(char *dir,int mode) {
	int i,j,c,rc;
	char tdir[256];
	for (i=j=0;;i++) {
		c=dir[i];
		if (c=='/') {
			tdir[j]=0;
			if (access(tdir,F_OK)) {
				rc=jmkdir(tdir,mode);
				if (rc) return -1;
			}
			tdir[j++]='/';
		}
		else if (!c) {
			if (access(dir,F_OK)) {
				rc=jmkdir(dir,mode);
				return rc;
			}
			break;
		}
		else tdir[j++]=c;
	}
	return 0;
}

//
// JDIR_RM
//

static int isdir(char *dir) {
	int rc;
	struct stat st1;
	rc=lstat(dir,&st1);
	if (rc) return -1;
	if (S_ISDIR(st1.st_mode)) return 1;
	return 0;
}

int jdir_rm(char *dir) {
	DIR *d1;
	struct dirent *de1;
	char fn[256],*cp1,*cp2;
	int l,l2,rc,errs=0;
	int testing=0;
	if (isdir(dir)<=0) return -1;
	if (!dir || !dir[0]) {
		strcpy(fn,"./");
		l=2;
	}
	else {
		l=strlen(dir);
		memcpy(fn,dir,l);
		if (fn[l-1] != '/') fn[l++]='/';
		fn[l]=0;
	}
	d1=opendir(fn);
	if (!d1) return -1;
	while (1) {
		de1=readdir(d1);
		if (!de1) break;
		cp1=&fn[l];
		cp2=de1->d_name;
		if (cp2[0]=='.' && (!cp2[1] || (cp2[1]=='.' && cp2[2]==0))) continue;
		l2=0;
		while ((*(cp1++)=*(cp2++))) l2++;
		rc=isdir(fn);
		if (rc==1) {
			rc=jdir_rm(fn);
			if (rc) errs++;
		}
		else {
			rc=unlink(fn);
			if (testing) printf("unlinking %s - rc=%d\n",fn,rc);
			if (rc) errs++;
		}
	}
	closedir(d1);
	rc=rmdir(dir);
	if (testing) printf("rmdiring %s - rc=%d\n",dir,rc);
	if (rc) errs++;
	return (errs==0)?0:-1;
}

//
// JDIR_CP
//

static int linkfile(char *srcdir,char *fn,char *dstdir,mode_t mode) {
	int rc,l;
	char buf[256],path[256];
	if ( (l=strlen(srcdir)) && srcdir[l-1] != '/')
		snprintf(path,sizeof(path),"%s/%s",srcdir,fn);
	else
		snprintf(path,sizeof(path),"%s%s",srcdir,fn);
	rc=readlink(path,buf,sizeof(buf));
	if (rc<0) return -1;
	else buf[rc]=0;
	if ( (l=strlen(dstdir)) && dstdir[l-1] != '/')
		snprintf(path,sizeof(path),"%s/%s",dstdir,fn);
	else
		snprintf(path,sizeof(path),"%s%s",dstdir,fn);
	rc=symlink(buf,path);
	return rc;
}

static int cpfile(char *srcdir,char *ifn,char *dstdir,mode_t mode) {
	char fn[256];
	int fd0,fd1,rc,rc2,retcode=0,bufl=(64*1024),l;
	char *buf=malloc(bufl);
	if ( (l=strlen(srcdir)) && srcdir[l-1] != '/')
		snprintf(fn,sizeof(fn),"%s/%s",srcdir,ifn);
	else
		snprintf(fn,sizeof(fn),"%s%s",srcdir,ifn);
	fd0=open(fn,O_RDONLY);
	if (fd0<0) return -1;

	if ( (l=strlen(dstdir)) && dstdir[l-1] != '/')
		snprintf(fn,sizeof(fn),"%s/%s",dstdir,ifn);
	else
		snprintf(fn,sizeof(fn),"%s%s",dstdir,ifn);

	//printf("copying %s\n",ifn);

	fd1=open(fn,O_WRONLY|O_CREAT,mode);
	if (fd1<0) {
		close(fd0);
		perror("open 2 failed");
		return -1;
	}
	while (1) {
		rc=read(fd0,buf,bufl);
		if (!rc) break;
		if (rc<0) {
			retcode=-1;
			break;
		}
		rc2=write(fd1,buf,rc);
		if (rc2 != rc) {
			retcode=-1;
			break;
		}
	}
	free(buf);
	close(fd0);
	close(fd1);
	return retcode;
}

int jdir_cp(char *idir,char *idir2) {
	char dir[256],dir2[256],*cp,tdir[256],tdir2[256];
	int rc,rc2=0,l,l2;
	DIR *d1;
	struct dirent *de1;
	struct stat st1;
	l=strlen(idir);
	memcpy(dir,idir,l+1);
	if (dir[l]!='/') {
		dir[l++]='/';
		dir[l++]=0;
	}
	l2=strlen(idir2);
	memcpy(dir2,idir2,l2+1);
	if (dir2[l2]!='/') {
		dir2[l2++]='/';
		dir2[l2++]=0;
	}
	if (isdir(dir2)<=0) {
		rc=lstat(dir,&st1);
		if (rc) return -1;
		rc=jmkdir(dir2,st1.st_mode&07777);
		if (rc) return -1;
	}
	d1=opendir(dir);
	if (!d1) return -1;
	while (1) {
		de1=readdir(d1);
		if (!de1) break;
		cp=de1->d_name;
		if (cp[0]=='.' && (!cp[1] || (cp[1]=='.' && cp[2]==0))) continue;
		snprintf(tdir,sizeof(tdir),"%s/%s",dir,cp);
		rc=lstat(tdir,&st1);
		if (rc) {
			fprintf(stderr,"lstat failed - %s - %s\n",cp,strerror(errno));
			rc2=-1;
			break;
		}
		if (S_ISREG(st1.st_mode)) {
			if (cpfile(dir,cp,dir2,st1.st_mode&07777)) {
				rc2=-1;
				break;
			}
		}
		else if (S_ISDIR(st1.st_mode)) {
			snprintf(tdir,sizeof(tdir),"%s/%s",dir,cp);
			snprintf(tdir2,sizeof(tdir2),"%s/%s",dir2,cp);
			if (jdir_cp(tdir,tdir2)) {
				rc2=-1;
				break;
			}
		}
		else if (S_ISLNK(st1.st_mode)) {
			if (linkfile(dir,cp,dir2,st1.st_mode&07777)) {
				rc2=-1;
				break;
			}
		}
	}
	closedir(d1);
	return 0;
}
