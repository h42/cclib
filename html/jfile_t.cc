#include <stdio.h>
#include "jstring.h"
#include "jfile.h"

int main() {
    jstring s;
    jfile f1("/etc/passwd");
    while (f1.gets(s)>0) {
	puts(s.cstr());
    }
    return 0;
}
