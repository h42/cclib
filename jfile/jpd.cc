#include <stdio.h>
#include <stdlib.h>
#include "../str/str.h"
#include "jfile.h"

int main() {
    jfile f3("/etc/group");
    const char *buf=f3.getFile();
    if (buf) puts(buf);
    /*
    str s;
    while (f1.gets(s)>0) {
	puts(s.cstr());
    }
    jfile f2("temp","w");
    f2.puts("Hey now!\n");
    f2.close();
    system("cat temp");
    */
    return 0;
}
