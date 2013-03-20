#include <stdio.h>
#include "../str/str.h"
#include "jfile.h"

int main() {
    str s;
    jfile f1("/etc/passwd");
    while (f1.gets(s)>0) {
	puts(s.cstr());
    }
    return 0;
}
