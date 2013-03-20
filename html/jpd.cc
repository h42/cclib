#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned int uint;

void html() {
    puts("Content-type: text/html; charset=utf-8\n");
    puts("<!DOCTYPE HTML>");
    puts("<html>");
    puts("<head>");
    puts("<title>");
    puts("form");
    puts("</title>");
    puts("</head>");
    puts("<body>");
    puts("<h1>Hey Now!</h1>");
    puts("</body>");
    puts("</html>");

}

int main() {
    html();
}
