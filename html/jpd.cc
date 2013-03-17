#include <stdio.h>
#include <malloc.h>
#include "jstring.h"
#include "vector"

using namespace std;

typedef unsigned char uchar;
typedef unsigned int uint;

int test_jstring() {
    jstring a("Hey "), b("Now"),c;
    puts("\nCOPY");
    c=a;
    a.display();
    c.display();
    c=b;
    b.display();
    c.display();

    puts("\nCONCAT");
    c=a;
    c.concat(b);
    c.display();
    c.concat('x').display();
    return 0;
}

void tvector() {
    vector<jstring> jv(5);
    jstring js(80);
    for (uint i; i<jv.size(); i++) {
	jv[i].format("hey %d", i);
	jv[i].display();
    }
}

int main() {
    tvector();
    //test_jstring();
}
