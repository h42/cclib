#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <my/str.h>
#include <my/jfile.h>
#include <my/parse.h>
#include <my/jsock.h>
//#include <my/log.h>

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
    "<h2>%s</h2>\n"
    "</body>\n"
    "</html>\n"
   ;

int errmsg(jsock &sk, const char *s) {
    char buf[4096];
    int rc = snprintf(buf,sizeof(buf),msg,s);
    return sk.write(buf,rc);
}

const char *root = "/www";

struct EXTS {
    const char *fext, *mtype;
};
EXTS ext[] = {
    {"gif", "image/gif" }
    ,{"jpg", "image/jpg" }
    ,{"jpeg","image/jpeg"}
    ,{"png", "image/png" }
    ,{"ico", "image/ico" }
    ,{"zip", "image/zip" }
    ,{"gz",  "image/gz"  }
    ,{"tar", "image/tar" }
    ,{"htm", "text/html" }
    ,{"html","text/html" }
    ,{0,0}
};

const char *getmimetype(const char *fn) {
    int i, l, l2=strlen(fn);
    for (i=0; ext[i].fext ;i++) {
        l=strlen(ext[i].fext);
        if (l2>=l && !strcasecmp(&fn[l2-l], ext[i].fext)) return ext[i].mtype;
    }
    return "";
}

int client(jsock &js) {
    char buf[4096];
    js.read(buf,sizeof(buf)-1);
    puts(buf);
    parse p(buf);
    str s, iop, ifn, iver, ofn;
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

    if (iop == "GET") { //|| iop=="POST") {
        str ofn;
        if (ifn=="/") ifn="/index.html";
        ofn=root + ifn;
        ofn.display();
        // MOVE TO OUTER SCOPE and use open to catch errors

        int acc;
        if (access(ofn.cstr(),X_OK)==0) acc=2;
        else if (access(ofn.cstr(),R_OK)==0) acc=1;
        else {
            errmsg(js,"Unable to access file");
            return -1;
        }
        printf("acc=%d\n",acc);

        if (acc==1) {
            jfile f(ofn.cstr());
            const char *buf2=f.getFile();
            if (!buf2) {
                errmsg(js,"getFile failed");
                return -1;
            }
            snprintf(buf, sizeof(buf),
                "HTTP/1.1 200 OK\nConnection: close\nContent-Type: %s\n\n",
                getmimetype(ofn.cstr()));
            js.write(buf, strlen(buf));
            js.write(buf2, strlen(buf2));
        }
        else if (acc==2) {
            errmsg(js,"CGI Not Implmented ");
            return -1;
        }
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
