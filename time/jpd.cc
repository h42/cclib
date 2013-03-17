#include <stdio.h>
#include <time.h>



int main() {
    time_t t0=time(0);
    struct tm tm;
    tm.tm_hour=tm.tm_min=tm.tm_sec=0;
    tm.tm_mday = 12;
    tm.tm_mon = 5-1;
    tm.tm_year=2028-1900;
    tm.tm_isdst = 1;
    time_t t1 = mktime(&tm);
    printf("%ld days to go!\n",(t1-t0)/(60*60*24));
    printf("%ld\n", sizeof(long));
    return 0;
}
