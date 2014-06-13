#include <stdio.h>

void prolog(const char *title) {
    printf("Content-type: text/html; charset=utf-8\r\n\r\n"
	   "<!DOCTYPE HTML>\r\n"
	   "<html>\r\n"
	   "<head>\r\n"
	   "<title>\r\n"
	   "%s\r\n"
	   "</title>\r\n"
	   "</head>\r\n"
	   "<body>\r\n"
        ,title);
}

void epilog() {
    printf("/body\r\n"
           "/html\r\n");
}

void html() {
    puts("<h3>Hey Now!!!</h3");
}

int main() {
    prolog("test");
    html();
    epilog();
}
