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

void t3() {
    str s="123456", s2="123456";
    (s+'q').display();
    puts ( s!=s2 ? "true" : "false");
    //puts ( s<"123456 " ? "true" : "false");
}

int main() {
    //t1();
    //t2();
    t3();
    return 0;
}
