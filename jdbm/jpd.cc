#include <stdio.h>
#include <string.h>
#include "jdbm.h"

int main() {
    puts("hey");
    jdbm db;
    int i,rc;
    char key[256],value[256];
    rc = db.open("temp.jdbm","n",0660);
    for (i=0;i<10;i++) {
        sprintf(key,"key %d",i);
        sprintf(value,"value %d",i);
        rc=db.insert(key, strlen(key)+1, value,strlen(value)+1);
        if (rc) fprintf(stderr,"insert failed; i=%d\n",i);
    }
    for (i=0;i<10;i++) {
        sprintf(key,"key %d",i);
        char *dptr=(char *)db.get(key,strlen(key)+1);
        if (!dptr) break;
        puts(dptr);
    }
    return 0;
}
