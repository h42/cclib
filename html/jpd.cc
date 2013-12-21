#include <stdio.h>
#include <ctype.h>
#include <jfile.h>
#include <str.h>
#include <vec.h>

typedef unsigned char uchar;
typedef unsigned int uint;

int sub(int x=7,int y=0) {
    printf("x=%d y=%d\n",x,y);
    return 0;
}

#define dsub(...) sub(__VA_ARGS__)

int main() {

    dsub(1,2);
    dsub();
    dsub(3);
    return 0;

    str s;
    jfile f;
    f.open("/etc/passwd");
    while (f.gets(s)>0) {
        s.display();
    }
    return 0;
}
