#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <inttypes.h>
#include <ctype.h>

using namespace std;

//ccinclude
#include <vector>
#include <sqlite3.h>

class sql {
public:
    sql(const char *fn=0);
    ~sql();
    int  open(const char *);
    void close();
    int  exec(const char *s);
    sqlite3 *getdb() {return zdb;}
    char zerr[256];
private:
    sqlite3 *zdb;
    int zopened;
};

struct vlnode {
    char name[64];
    void *addr;
    int type; // 0=int  1=int64  2=text
};

class sql;

class vlist {
public:
    vlist() {zcnt=0;}
    void add(const char *name, void *vp, int type);
    void show();
    vlnode *find(const char *s);
private:
    std::vector<vlnode> vn;
    int zcnt;
};

class sql_stmt {
public:
    sql_stmt() {zprepared=0;colcnt=0;bindcnt=0;}
    ~sql_stmt() {if (zprepared) finalize();}
    int prepare(sql db, const char *sql);
    int prepare(sql db, const char *sql, vlist &vl);
    void showprep();
    int finalize();
    int bind_text(int col, char *buf);
    int bind_int(int col, int val);
    int bind_int64(int col, long val);
    int bind();
    int step();
    int64_t column_int64(int col);
    int column_int(int col);
    const char *column_text(int col);
    int reset();
private:
    sqlite3_stmt *zstmt;
    int zprepared;
    int colcnt,bindcnt;
    vlnode *coltab[128], *bindtab[128];
};

//ccinclude

vlnode *vlist::find(const char *s) {
    int i;
    for (i=0; i<zcnt; i++) if (!strcmp(vn[i].name,s)) return &vn[i];
    return 0;
}

void vlist::add(const char *name, void *addr, int type) {
    vlnode v1;
    strncpy(v1.name,name,sizeof(v1.name)-1);
    v1.name[sizeof(v1.name)-1]=0;
    v1.addr=addr;
    v1.type=type;
    vn.push_back(v1);
    zcnt++;
}

void vlist::show() {
    int i;
    for (i=0;i<zcnt;i++) printf("%10s - %p %s - %d\n",
	vn[i].name, vn[i].addr, (char *)vn[i].addr, vn[i].type);
}

sql::sql(const char *fn) {
    zopened=0;
    memset(zerr,0,sizeof(zerr));
    if (fn) open(fn);
}

sql::~sql() {
    if (zopened) close();
}

int sql::open(const char *fn) {
    int rc;
    rc=sqlite3_open(fn,&zdb);
    if (rc==SQLITE_OK) zopened=1;
    return rc;
}

void sql::close() {
    if (zopened) {
	sqlite3_close(zdb);
	zopened=0;
    }
}

int sql::exec(const char *s) {
    int rc;
    char *err;
    if (!zopened) return 1;
    zerr[0]=0;
    err=0;
    rc = sqlite3_exec(zdb,s,0,0,&err);
    if (err) {
	strncpy(zerr,err,sizeof(zerr)-1);
	sqlite3_free(err);
    }
    return rc;
}

void sql_stmt::showprep() {
    printf("%d %d\n",colcnt,bindcnt);
    for (int i=0;i<colcnt;i++)
	printf("col %p %s\n",coltab[i],coltab[i]->name);
    for (int i=0;i<bindcnt;i++)
	printf("bind %p %s\n",bindtab[i],bindtab[i]->name);
}

int sql_stmt::prepare(sql db, const char *sql, vlist &vl) {
    int i,j,k,c,vartype,state,l = strlen(sql);
    char *sql2 = (char *) alloca(l+1);
    char ts[256];
    vlnode *vln;
    colcnt = bindcnt = 0;
    for (i=0,j=0,state=0 ;i<l ;i++) {
	c=sql[i];
	switch(state) {
	case 0:
	    if (c==':') {
		state=1;
		vartype=1;
		k=0;
	    }
	    else if (c=='?') {
		sql2[j++]=c;
		state=1;
		vartype=2;
		k=0;
	    }
	    else sql2[j++]=c;
	    break;

	case 1:
	    if (!isalnum(c)) {
		ts[k]=0;
		sql2[j++]=c;
		state=0;
		vln = vl.find(ts);
		if (vln==0) return -1;
		if (vartype == 1) coltab[colcnt++] = vln;
		else  bindtab[bindcnt++] = vln;
	    }
	    else ts[k++]=c;
	    break;

	default:
	    break;
	}

    }
    sql2[j]=0;
    puts(sql);
    puts(sql2);
    return prepare(db, sql2);
}
int sql_stmt::prepare(sql db, const char *sql) {
    int rc;
    rc=sqlite3_prepare_v2(db.getdb(),sql,-1,&zstmt,0);
    zprepared = rc==0 ? 1 : 0;
    return rc;
}

int sql_stmt::bind_text(int col, char *buf) {
   return zprepared ? sqlite3_bind_text(zstmt,col,buf,-1,SQLITE_STATIC) : -1;
}

int sql_stmt::bind_int64(int col, long val) {
   return zprepared ? sqlite3_bind_int64(zstmt, col, val) : -1;
}

int sql_stmt::bind_int(int col, int val) {
   return zprepared ? sqlite3_bind_int(zstmt, col, val) : -1;
}

int sql_stmt::bind() {
    vlnode *vln;
    int ind=1;
    for (int i=0; i<bindcnt; i++) {
	vln = bindtab[i];
	switch (vln->type) {
	case 0:
	    if (bind_int(ind++, *(int *)(vln->addr))) return -1;
	    break;
	case 2:
	    if (bind_text(ind++, (char *)(vln->addr))) return -1;
	    break;
	}
    }
    return 0;
}

int sql_stmt::column_int(int col) {
    return sqlite3_column_int(zstmt,col);
}

int64_t sql_stmt::column_int64(int col) {
    return sqlite3_column_int64(zstmt,col);
}

const char * sql_stmt::column_text(int col) {
    return (const char *)sqlite3_column_text(zstmt,col);
}

int sql_stmt::step() {
   return zprepared ? sqlite3_step(zstmt) : 1;
}

int sql_stmt::reset() {
   return zprepared ? sqlite3_reset(zstmt) : 1;
}

int sql_stmt::finalize() {
    int rc=0;
    if (zprepared) {
       rc=sqlite3_finalize(zstmt);
       zprepared=0;
    }
   return rc;
}

