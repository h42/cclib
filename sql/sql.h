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

