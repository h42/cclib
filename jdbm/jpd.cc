#include <stdio.h>
#include <string.h>
#include "jdbm.h"

int main() {
    puts("hey");
    jdbm db;
    int i,rc;
    char key[256],value[256];
    rc = db.open("temp.jdbm","n",0660);
    for (i=0;i<100000;i++) {
        sprintf(key,"key %d",i);
        sprintf(value,"value %d",i);
        rc=db.insert(key, strlen(key)+1, value,strlen(value)+1);
        if (rc) fprintf(stderr,"insert failed; i=%d\n",i);
    }

    sprintf(key,"key %d",5);
    rc=db.remove(key, strlen(key)+1);
    printf("remove rc = %d\n",rc);

    for (i=0;i<10;i++) {
        sprintf(key,"key %d",i);
        char *dptr=(char *)db.get(key,strlen(key)+1);
        if (!dptr) {
            printf("record %d is missing\n",i);
            continue;
        }
        puts(dptr);
    }

    char *buf=(char *)db.first();
    puts("");
    i=100;
    while (buf && ++i<110) {
        puts(buf);
        buf=(char *)db.next();
    }
    return 0;
}
