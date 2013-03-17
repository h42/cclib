
//ccinclude
#include <time.h>

typedef unsigned long Jtime;
class jtime {
public:
    Jtime set(int yr, int mon=1, int day=1, int hr=0, int min=0, int sec=0, int usec=0);
    time_t set_unix(int yr, int mon=1, int day=1, int hr=0, int min=0, int sec=0);
private:
    time_t zunix;
    Jtime zt;
};
//ccinclude

Jtime
jtime::set(int yr, int mon, int day, int hr, int min, int sec, int usec) {
    zt = usec + (sec<<20) + (min<<26) + ((long)hr<<32) +
	 ((long)day<<37) + ((long)mon<<42) + ((long)yr<<47);
    return zt;
}

time_t
jtime::set_unix(int yr, int mon, int day, int hr, int min, int sec) {
    struct tm tm;
    tm.tm_hour=hr;
    tm.tm_min=min;
    tm.tm_sec=sec;
    tm.tm_mday = day;
    tm.tm_mon = mon;
    tm.tm_year=yr-1900;
    tm.tm_isdst = 0;
    zunix = mktime(&tm);
    return zunix;
}

