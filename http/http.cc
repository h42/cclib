#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <my/str.h>
#include <my/jfile.h>
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

const char *root = "/www";

/*
extensions = [("gif", "image/gif" ),
        ("jpg", "image/jpg" ),
        ("jpeg","image/jpeg"),
        ("png", "image/png" ),
        ("ico", "image/ico" ),
        ("zip", "image/zip" ),
        ("gz",  "image/gz"  ),
        ("tar", "image/tar" ),
        ("htm", "text/html" ),
        ("/", "text/html" ),  -- for index.html
        ("html","text/html" ) ] :: [(String,String)]
*/

int client(jsock &js) {
char buf[4096];
    js.read(buf,sizeof(buf)-1);
    puts(buf);
    parse p(buf);
    str s, iop, ifn, iver;
    int rc;
    rc=p.word(iop);  p.spaces();
    rc=p.word(ifn);  p.spaces();
    rc=p.word(iver);
    rc=p.eol();
    while(!p.eod()) {
        rc=p.line(s);
        if (rc<0) break;
        s.display();
        if (s.length()==0) break;
    }

    if (iop == "GET" || iop=="POST") {
        str ofn;
        if (ifn=="/") ifn="/index.html";
        ofn=root + ifn;
        ofn.display();
        jfile f(ofn.cstr()); // MOVE TO OUTER SCOPE and use open to catch errors
        const char *buf=f.getFile();
        if (buf) js.write(buf, strlen(buf));
    }
    else js.write(msg, strlen(msg));
    //js.sync();
    return 0;
}

int main() {
	jsock js;
        js.run(8080,client);
	return 0;
}
