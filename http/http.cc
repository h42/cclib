#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <my/str.h>
#include <my/parse.h>
#include <my/jsock.h>

const char *msg=
    "HTTP/1.1 200 OK\n"
    "Connection: close\n"
    "Content-type: text/html; charset=utf-8\n\n"

    "<!DOCTYPE HTML>\n"
    "<html>\n"
    "<head>\n"
    "<meta http_equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n"
    "<title>\n"
    "Test Page\n"
    "</title>\n"
    "</head>\n"
    "<body>\n"
    "<h2>Hey Now!</h2>\n"
    "</body>\n"
    "</html>\n"
   ;

char *root = "/www";

int client(jsock &js) {
char buf[4096];
    js.read(buf,sizeof(buf)-1);
    puts(buf);
    parse p(buf);
    str s, iop, ifn, iver;
    int rc,state=0;
    rc=p.word(iop);  p.spaces();
    rc=p.word(ifn);  p.spaces();
    rc=p.word(iver);
    rc=p.eol();
    while(!p.eod()) {
        rc=p.line(s);
        s.display();
        if (s.length()==0) break;
    }

    if (iop == "GET") {
        str ofn;
    }

    js.write(msg, strlen(msg));
    //js.sync();
    return 0;
}

int main() {
	jsock js;
        js.run(8080,client);
	return 0;
}
