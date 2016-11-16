#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "ntp-structure.h"





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

void print_unix_to_hr(struct timeval *tv)
{
  time_t t2 = (time_t) tv->tv_sec;
  __darwin_time_t milliseconds = tv->tv_usec / 1000;
  struct tm* tm_info;
  tm_info = localtime(&t2);
  char buffer[200];
  char milliBuffer[100];

  sprintf(milliBuffer, "%ld", milliseconds);
  strftime(buffer, 30, "%Y-%m-%d %H:%M:%S.", tm_info);

  strcat(buffer, milliBuffer);

  printf("Time is: %s\n", buffer);
}

/*int main()
{
    struct ntp_time_t ntp;
    struct timeval tv = {0};


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


    //convert to human readable

    time_t t2 = (time_t) tv.tv_sec;
    uint32_t milliseconds = tv.tv_usec / 1000;
    struct tm* tm_info;
    tm_info = localtime(&t2);
    char buffer[200];
    char milliBuffer[100];

    sprintf(milliBuffer, "%u", milliseconds);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S.", tm_info);

    strcat(buffer, milliBuffer);

    printf("Time is: %s\n", buffer);

}*/
