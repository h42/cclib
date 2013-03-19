#include <my/str.h>

void t1() {
    str s1("string1");
    str s2("string2");
    str s3 = s1+s2;
    printf("%s\n",s3.cstr());
}

int main() {
    t1();
    return 0;
}
