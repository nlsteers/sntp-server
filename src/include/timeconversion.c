#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

struct ntp_time_t {
    uint32_t   second;
    uint32_t   fraction;
};

void ntp_time_to_unix_time(struct ntp_time_t *ntp, struct timeval *tv)
{
    tv->tv_sec = ntp->second - 2208988800; // the seconds from Jan 1, 1900 to Jan 1, 1970
    tv->tv_usec = (uint32_t)( (double)ntp->fraction * 1000000 / (double)(1LL<<32) );
}

void unix_time_to_ntp_time(struct timeval *tv, struct ntp_time_t *ntp)
{
    ntp->second = tv->tv_sec + 2208988800;
    ntp->fraction = (uint32_t)( (double)(tv->tv_usec+1) * (double)(1LL<<32) * 0.000001 );
}

int main()
{
    struct ntp_time_t ntp;
    struct timeval tv = {0};
    time_t now;
    struct tm ts;
    char buf[80];

    // get time unix time via gettimeofday
    gettimeofday(&tv, NULL);
    printf("UNIX Time: %ld %ld\n", tv.tv_sec, tv.tv_usec);

    // convert unix time to ntp time
    unix_time_to_ntp_time(&tv, &ntp);
    printf("NTP Time: %u %u\n", ntp.second, ntp.fraction);

    // convert ntp time back to unix time to see whether they are same
    ntp_time_to_unix_time(&ntp, &tv);
    printf("UNIX Time: %ld %ld\n", tv.tv_sec, tv.tv_usec);


    unsigned long long millisecondsSinceEpoch = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;

    printf("Milliseconds since epoch (UNIX) %llu\n", millisecondsSinceEpoch);

    // Get current time
    time(&now);

    printf("Time: %lu \n", now);
    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&now);

    printf("localtime: %d %d %d \n", ts.tm_hour, ts.tm_min, ts.tm_sec);

    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    printf("%s\n", buf);
    return 0;



}
