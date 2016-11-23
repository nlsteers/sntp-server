

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../include/time-conversion.h"

#define PORT 123
//#define PORT 4950

int main(int argc, char *argv[]) {

    struct sntpPacket sendPacket, recPacket;
    struct ntp_time_t ntp, originntp, transmitntp, receiventp = {0};
    struct timeval tv, arrival, origintv, transmittv, receivetv = {0};


    memset(&sendPacket, 0, sizeof(struct sntpPacket));
    memset(&recPacket, 0, sizeof(struct sntpPacket));


//set flags

    sendPacket.LI = 0;
    sendPacket.VN = 4;
    sendPacket.MODE = 3;

    int sockfd;

    socklen_t addr_len;
    struct hostent *he;
    struct sockaddr_in their_addr;

    // THIS IS TEMPORARY, REMOVE ON RELEASE
    //char hn[10] = "localhost";
    //char hn[18] = "0.uk.pool.ntp.org";

/*  Loop for hostname selection */

    int c;
    int count;

    char hn[256];

    printf("Enter hostname: ");
    for(count = 0; (c = getchar()) != '\n' && count < 256; count++) {
        hn[count] = (char) c;
    }
    hn[count] = '\0';



    if ((he = gethostbyname(hn)) == NULL) {
        perror("SNTP gethostbyname error");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("SNTP socket error");
        exit(1);
    }

    memset(&their_addr, 0, sizeof(their_addr));
    /* zero struct */
    their_addr.sin_family = AF_INET;
    /* host byte order .. */
    their_addr.sin_port = htons(PORT);
    /* .. short, netwk byte order */
    their_addr.sin_addr = *((struct in_addr *) he->h_addr);


//get reference time

    get_ntp_time(&tv, &ntp);

//change byte order

    long tempSeconds = htonl(ntp.second);
    long tempFractions = htonl(ntp.fraction);

    sendPacket.trans_ts_sec = (uint32_t) tempSeconds;
    sendPacket.trans_ts_frac = (uint32_t) tempFractions;

//send packet
    if ((sendto(sockfd, &sendPacket, sizeof(sendPacket), 0,
                (struct sockaddr *) &their_addr, sizeof(struct sockaddr))) == -1) {
        perror("SNTP sendto error");
        exit(1);
    }

//set address length
    addr_len = sizeof(struct sockaddr_in);

    memset(&tv, 0, sizeof(struct timeval));

//listen for response

    if ((recvfrom(sockfd, &recPacket, sizeof(recPacket), 0,
                  (struct sockaddr *) &their_addr, &addr_len)) == -1) {
        perror("SNTP recvfrom error");
        exit(1);
    }
    get_time(&arrival);


//change byte order

    originntp.second = ntohl(recPacket.origin_ts_sec);
    originntp.fraction = ntohl(recPacket.origin_ts_frac);

    transmitntp.second = ntohl(recPacket.trans_ts_sec);
    transmitntp.fraction = ntohl(recPacket.trans_ts_frac);

    receiventp.second = ntohl(recPacket.recv_ts_sec);
    receiventp.fraction = ntohl(recPacket.recv_ts_frac);


    ntp_time_to_unix_time(&originntp, &origintv);

    ntp_time_to_unix_time(&transmitntp, &transmittv);

    ntp_time_to_unix_time(&receiventp, &receivetv);

//DELAY--------------------------------------------

    double dSec, dUsec;
    dSec = ((double) arrival.tv_sec - (double) origintv.tv_sec) -
           ((double) transmittv.tv_sec - (double) receivetv.tv_sec);
    dUsec = ((double) arrival.tv_usec - (double) origintv.tv_usec) -
            ((double) transmittv.tv_usec - (double) receivetv.tv_usec);

    double dTotal = dSec + (dUsec / 1000000);

//------------------------------------------------------

//OFFSET------------------------------------------------

    double oSec, oUsec;
    oSec = ((double) receivetv.tv_sec - (double) origintv.tv_sec) +
           ((double) transmittv.tv_sec - (double) arrival.tv_sec) / 2;
    oUsec = ((double) receivetv.tv_usec - (double) origintv.tv_usec) +
            ((double) transmittv.tv_usec - (double) arrival.tv_usec) / 2;

    double oTotal = oSec + (oUsec / 1000000);

//------------------------------------------------------

//convert to UNIX time

    memset(&ntp, 0, sizeof(struct ntp_time_t));

    ntp.second = ntohl(recPacket.recv_ts_sec);
    ntp.fraction = ntohl(recPacket.recv_ts_frac);

    ntp_time_to_unix_time(&ntp, &tv);

//convert to human readable
    print_unix_to_hr(&tv);
    printf("+ %f +/- %f %s(%s)\n", oTotal, dTotal, hn, inet_ntoa(their_addr.sin_addr));

    close(sockfd);
    return 0;
}

