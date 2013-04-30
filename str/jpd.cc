#include <my/str.h>

void t1() {
    str s1("string1");
    str s2("string2");
    str s3 = s1+s2;
    char c = s3[0];
    printf("%s %c\n",s3.cstr(),c);
}

void t2() {
    str s("");
    for (int i=0;i<26;i++) s+('a'+i);
    s.display();
    s="";
    for (int i=0;i<10;i++) s+('0'+i);
    s.display();
}

int main() {
    //t1();
    t2();
    return 0;
}
