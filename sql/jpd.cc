#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <inttypes.h>
#include <ctype.h>
#include <vector>
#include "sql.h"

using namespace std;

int main() {
    int i,rc;
    sql db("temp.db");
    sql_stmt stmt;

    vlist vl;
    char t1[80]="hey now";
    int i1;
    vl.add("t1",t1,2);
    vl.add("i1",&i1,0);
    vl.show();

    rc = db.exec("begin");
    rc = db.exec("drop table t1");
    rc = db.exec("create table t1(i1 integer, t1 text)");

    rc=stmt.prepare(db,"insert into t1 values(?i1 ,?t1 )",vl);
    stmt.showprep();
    for (i=1; i<100000; i++) {
	//stmt.bind_int(1,i);
	//stmt.bind_text(2,t1);
	i1=i;
	stmt.bind();
	rc=stmt.step(); //printf("rc=%d\n",rc);
	stmt.reset();
    }
    stmt.finalize();
    rc = db.exec("commit");

    rc = db.exec("begin");
    rc=stmt.prepare(db,"select i1,t1 from t1");
    printf("prepare rc = %d\n",rc);
    if (rc) return rc;
    rc=stmt.step();
    printf("rc=%d i1=%d - t1=%s\n",
	rc,stmt.column_int(0),stmt.column_text(1));
    stmt.finalize();
    rc = db.exec("commit");
    return rc;
}

