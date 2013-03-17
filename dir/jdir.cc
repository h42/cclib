#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

int getdir(const char *dir, vector< pair<char,string> > &v, int flag) {
    int l,l2,rc;
    struct stat st1;
    DIR *d1;
    struct dirent *de1;
    char fn[256],*cp1,*cp2,type;
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
	if (rc) return -1;
	if (S_ISREG(st1.st_mode)) type = ' ';
	else if (S_ISDIR(st1.st_mode)) {
	    if (flag == 0) type = '/';
	    else {
		    getdir(fn, v, flag);
		    continue;
	    }
	}
	else if (S_ISLNK(st1.st_mode)) type = '@';
	else type = '?';

	v.push_back(make_pair(type,fn));
    }
    closedir(d1);
    return 0;
}

int main() {
    unsigned int i;
    string s = "./";
    vector < pair<char,string> > v;
    getdir(s.c_str(), v, 1);
    for (i=0; i < v.size(); i++)
	    cout << v[i].first << " " << v[i].second << endl;
    return 0;
}
