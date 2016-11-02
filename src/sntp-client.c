#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>

struct ntp_time_t {
    uint32_t   second;
    uint32_t    fraction;
};

void convert_ntp_time_into_unix_time(struct ntp_time_t *ntp, struct timeval *unix)
{
    unix->tv_sec = ntp->second - 0x83AA7E80; // the seconds from Jan 1, 1900 to Jan 1, 1970
    unix->tv_usec = (uint32_t)( (double)ntp->fraction * 1.0e6 / (double)(1LL<<32) );
}

void convert_unix_time_into_ntp_time(struct timeval *unix, struct ntp_time_t *ntp)
{
    ntp->second = unix->tv_sec + 0x83AA7E80;
    ntp->fraction = (uint32_t)( (double)(unix->tv_usec+1) * (double)(1LL<<32) * 1.0e-6 );
}

int main()
{
    struct ntp_time_t ntp;
    struct timeval unix;

    // get time unix time via gettimeofday
    gettimeofday(&unix, NULL);
    printf("UNIX Time: %ld %ld\n", unix.tv_sec, unix.tv_usec);

    // convert unix time to ntp time
    convert_unix_time_into_ntp_time(&unix, &ntp);
    printf("NTP Time: %ld %ld\n", ntp.second, ntp.fraction);

    // convert ntp time back to unix time to see whether they are same
    convert_ntp_time_into_unix_time(&ntp, &unix);
    printf("UNIX Time: %ld %ld\n", unix.tv_sec, unix.tv_usec);
}
