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

char buf[4096];
str zfn(256), zop(20), zquery(256);

int client(jsock &js) {
    js.read(buf,sizeof(buf)-1);
    parse p(buf);
    vec<str> v;
    p.words(v);
    v[0].display();
    puts(buf);
    js.write(msg, strlen(msg));
    //js.sync();
    return 0;
}

int main() {
	jsock js;
        js.run(8080,client);
	return 0;
}
