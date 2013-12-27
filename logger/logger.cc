#include <stdarg.h>
#include <time.h>

//ccinclude
#ifndef _LOGGER_H
#define _LOGGER_H
#ifndef _STDIO_H
#include <stdio.h>
#endif

class logger {
public:
    logger(const char *fn) {open(fn);}
    logger() {fh=0;}
    FILE *open(const char *fn="temp.log");
    ~logger() {if (fh) fclose(fh);}
    int log(const char *fmt, ...);
private:
    char fn[256];
    FILE *fh;
};

#endif
//ccinclude

FILE *
logger::open(const char *fn) {
    fh = fopen(fn,"w");
    return fh;
}

int
logger::log(const char *fmt, ...) {
    if (!fh) return -1;
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    time_t t1=time(0);
    tm tm1;
    localtime_r(&t1, &tm1);
    char tbuf[40];
    snprintf(tbuf, sizeof(tbuf), "%2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
        tm1.tm_mon+1, tm1.tm_mday, tm1.tm_year%100,
        tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
    fprintf(fh,"%s %s\n",tbuf,buf);
    return 0;
}

int xmain() {
    return 0;
}
