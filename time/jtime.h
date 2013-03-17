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
