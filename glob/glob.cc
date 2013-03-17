#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <vector>

using namespace std;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef const char * cchar;

class glob;

#define MAXBRACE 10

class glob_item {
public:
    glob_item() { ztype=-1; }
    void init(int x) { memset(zgs,x,256); ztype=0; zneg=x; }
    void wild() { memset(zgs,1,256); ztype=0; }
    void add(int c) { zgs[c] = zneg ? 0 : 1; }
    int  elem(int c) { return zgs[c]; }
    void dump();

    int ztype;
    int zneg;
    uchar zgs[256];
    glob *zglob[MAXBRACE];
    int zglobs;
};

#define MS 100
class glob {
public:
    glob() { init(); }
    int compile(cchar s,int brace);
    int run(cchar,int=0,int=0);
    void dump();
private:
    void init() {zlen=0;}
    void add2glob(glob_item &g) {v.push_back(g); zlen++; }
    int push(int x);

    vector<glob_item> v;
    int zlen,zsize;
};

void glob_item::dump() {
    int i;
    printf("type=%d neg=%d",ztype,zneg);
    for (i=0;i<256;i++) if (i>='a' && i<='z' && zgs[i] ) printf(" %c",i);
    putchar(10);
}

void glob::dump() {
    int i;
    for (i=0;i<zlen;i++) {
	v[i].dump();
    }
}

int glob::run(cchar s,int i,int j) {
    int rc,l2=strlen(s);
    for (i=0;i<zlen;i++) {
	if (j==l2) return -1;
	if (v[i].ztype == 0) {
	    if (v[i].zgs[(int)s[j++]]==0) return -1;
	}
	else if (v[i].ztype==1) {
	    for (;j<l2;j++) {
		rc = run(&s[j],i+1,j);
		if (rc == 0) return 0;
	    }
	    return -1;
	}
    }
    if (j != l2) return -1;
    return 0;
}

int glob::compile(cchar s,int brace) {
    int i,c,l = strlen(s),state=0,bl;
    glob_item gl;
    glob *tgl;
    for (i=0; i<l; i++) {
	if (state==0) {
	    c=s[i];
	    if (c=='*') {
		gl.init(0);
		gl.ztype = 1;
		add2glob(gl);
	    }
	    else if (c=='?') {
		gl.wild();
		add2glob(gl);
	    }
	    else if (c=='[') {
		state=1;
		bl=0;
		gl.init(0);
	    }
	    else if (c=='{') {
		if (brace) return -1;
		state=2;
		gl.ztype=2;
		gl.zglobs=0;
		tgl = new glob;
	    }
	    else if (c=='}') {
		if (!brace) return -1;
		break;
	    }
	    else {
		gl.init(0);
		gl.add(c);
		add2glob(gl);
	    }
	}
	else if (state==1) {
	    if (c==']') {
		state=0;
		add2glob(gl);
	    }
	    else if (bl==0 && c=='!') gl.init(1);
	    else gl.add(c);
	}
    }
    return (state==0) ? l-i : -1;
}

int main() {
    glob gl;
    int rc = gl.compile("a?e",0);
    printf("compile rc=%d\n",rc);
    gl.dump();
    rc=gl.run("abe");
    printf("run rc=%d\n",rc);
    return 0;
}
