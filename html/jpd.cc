#include <stdio.h>
#include <ctype.h>
#include <jfile.h>
#include <str.h>
#include <vec.h>

typedef unsigned char uchar;
typedef unsigned int uint;

int main() {
    str s;
    jfile f=jfile("/etc/passwd");
    while (f.gets(s)>0) {
        s.display();
    }
    return 0;
}
