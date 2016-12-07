//
// Created by Nathaniel Steers on 20/11/2016.
//

#ifndef SNTP_COURSEWORK_TIME_CONVERSION_H
#define SNTP_COURSEWORK_TIME_CONVERSION_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include "ntp-time.h"
#include "sntp-packet.h"



//ref http://waitingkuo.blogspot.co.uk/2012/06/conversion-between-ntp-time-and-unix.html

void ntp_time_to_unix_time(struct ntp_time_t *ntp, struct timeval *tv) {


    tv->tv_sec = ntp->second - 2208988800; // the seconds from Jan 1, 1900 to Jan 1, 1970
    tv->tv_usec = (uint32_t) ((double) ntp->fraction * 1.0e6 / (double) (1LL << 32));


}

void unix_time_to_ntp_time(struct timeval *tv, struct ntp_time_t *ntp) {
    ntp->second = (uint32_t) (tv->tv_sec + 2208988800);
    ntp->fraction = (uint32_t) ((double) (tv->tv_usec + 1) * (double) (1LL << 32) * 1.0e-6);
}

//-------------------------------------------------------------------------------------------



void get_time(struct timeval *tv) {
    gettimeofday(tv, NULL);
}

void get_ntp_time(struct timeval *tv, struct ntp_time_t *ntp) {
    gettimeofday(tv, NULL);
    unix_time_to_ntp_time(tv, ntp);
}

void print_timestamp(struct timeval *tv) {
    time_t t2 = (time_t) tv->tv_sec;
    uint32_t milliseconds = (uint32_t) (tv->tv_usec / 1);
    struct tm *tm_info;
    tm_info = localtime(&t2);
    char b1[200];
    char b2[100];
    char b3[100];

    sprintf(b2, "%u", milliseconds);
    strftime(b1, 50, "%Y-%m-%d %H:%M:%S.", tm_info);

    strftime(b3, 10, " (%z)", tm_info);
    strcat(b1, b2);
    strcat(b1, b3);
    printf("%s ", b1);
}


void print_leap_positive_timestamp(struct timeval *tv) {
    time_t t2 = (time_t) (tv->tv_sec + 1);
    uint32_t milliseconds = (uint32_t) (tv->tv_usec / 1);
    struct tm *tm_info;
    tm_info = localtime(&t2);
    char b1[200];
    char b2[100];
    char b3[100];

    sprintf(b2, "%u", milliseconds);
    strftime(b1, 50, "%Y-%m-%d %H:%M:%S.", tm_info);

    strftime(b3, 10, " (%z)", tm_info);
    strcat(b1, b2);
    strcat(b1, b3);
    printf("%s ", b1);
}

void print_leap_negative_timestamp(struct timeval *tv) {
    time_t t2 = (time_t) (tv->tv_sec - 1);
    uint32_t milliseconds = (uint32_t) (tv->tv_usec / 1);
    struct tm *tm_info;
    tm_info = localtime(&t2);
    char b1[200];
    char b2[100];
    char b3[100];

    sprintf(b2, "%u", milliseconds);
    strftime(b1, 50, "%Y-%m-%d %H:%M:%S.", tm_info);

    strftime(b3, 10, " (%z)", tm_info);
    strcat(b1, b2);
    strcat(b1, b3);
    printf("%s ", b1);
}


void print_network_packet(struct sntpPacket *sntp) {
    printf("Packet contains: \n"
                   "Stratum: %u \n"
                   "Poll: %u \n"
                   "Precision: %i \n"
                   "Root Delay: %i \n"
                   "Root Dispersion: %u \n"
                   "Ref ID: %u \n"
                   "Reference Seconds: %u \n"
                   "Reference Fractions: %u \n"
                   "Origin Seconds: %u \n"
                   "Origin Fractions: %u \n"
                   "Received Seconds: %u \n"
                   "Received Fractions: %u \n"
                   "Transmit Seconds: %u \n"
                   "Transmit Fractions: %u \n\n\n",
           sntp->stratum,
           sntp->poll,
           sntp->precision,
           ntohl((uint32_t) sntp->root_delay),
           ntohl(sntp->root_dispersion),
           ntohl(sntp->ref_ID),
           ntohl(sntp->ref_ts_sec),
           ntohl(sntp->ref_ts_frac),
           ntohl(sntp->origin_ts_sec),
           ntohl(sntp->origin_ts_frac),
           ntohl(sntp->recv_ts_sec),
           ntohl(sntp->recv_ts_frac),
           ntohl(sntp->trans_ts_sec),
           ntohl(sntp->trans_ts_frac));
}

double Log2(double n) {
    // log(n)/log(2) is log2.
    return log(n) / log(2);
}

int getCode(struct sntpPacket *refCode) {

    int code = 0;

    if(memcmp(&refCode->ref_ID, "ACST", 4) == 0){
        printf("Ref ID: ACST\n The association belongs to an anycast server\n");
        code = 10;
    }
    if(memcmp(&refCode->ref_ID, "AUTH", 4) == 0){
        printf("Ref ID: AUTH\n Server authentication failed\n");
        code =  11;
    }
    if(memcmp(&refCode->ref_ID, "AUTO", 4) == 0){
        printf("Ref ID: AUTO\n Autokey sequence failed\n");
        code =  12;
    }
    if(memcmp(&refCode->ref_ID, "BCST", 4) == 0){
        printf("Ref ID: BCST\n The association belongs to a broadcast server\n");
        code =  13;
    }
    if(memcmp(&refCode->ref_ID, "CRYP", 4) == 0){
        printf("Ref ID: CRYP\n Cryptographic authentication or identification failed\n");
        code =  14;
    }
    if(memcmp(&refCode->ref_ID, "DENY", 4) == 0){
        printf("Ref ID: DENY\n Access denied by remote server\n");
        code =  15;
    }
    if(memcmp(&refCode->ref_ID, "DROP", 4) == 0){
        printf("Ref ID: DROP\n Lost peer in symmetric mode\n");
        code =  16;
    }
    if(memcmp(&refCode->ref_ID, "RSTR", 4) == 0){
        printf("Ref ID: RSTR\n Access denied due to local policy\n");
        code =  17;
    }
    if(memcmp(&refCode->ref_ID, "INIT", 4) == 0){
        printf("Ref ID: INIT\n The association has not yet synchronized for the first time\n");
        code =  18;
    }
    if(memcmp(&refCode->ref_ID, "MCST", 4) == 0){
        printf("Ref ID: MCST\n The association belongs to a manycast server\n");
        code =  19;
    }
    if(memcmp(&refCode->ref_ID, "NKEY", 4) == 0){
        printf("Ref ID: NKEY\n No key found. Either the key was never installed or is not trusted\n");
        code =  20;
    }
    if(memcmp(&refCode->ref_ID, "RATE", 4) == 0){
        printf("Ref ID: RATE\n Rate exceeded. The server has temporarily denied access because the client exceeded the rate threshold\n");
        code =  21;
    }
    if(memcmp(&refCode->ref_ID, "RMOT", 4) == 0){
        printf("Ref ID: RMOT\n Somebody is tinkering with the association from a remote host running ntpdc\n");
        code =  22;
    }
    if(memcmp(&refCode->ref_ID, "STEP", 4) == 0){
        printf("Ref ID: STEP\n A step change in system time has occurred, but the association has not yet resynchronized\n");
        code =  23;
    }
    return code;
}

void get_reference_time(struct sntpPacket *sendPacket) {


    struct sntpPacket referencePacket;
    struct ntp_time_t ntp = {0};
    struct timeval tv, timeout = {0};

    long tempSeconds;
    long tempFractions;


    uint8_t stratumTemp;

    memset(&referencePacket, 0, sizeof(struct sntpPacket));


//set flags

    referencePacket.LI = 0;
    referencePacket.VN = 4;
    referencePacket.MODE = 3;

    int sockfd, error;

    socklen_t addr_len;
    struct hostent *he;
    struct sockaddr_in their_addr;


    char hn[18] = "ntp.uwe.ac.uk";

    error = 0;

    if ((he = gethostbyname(hn)) == NULL) {
        perror("Error resolving reference hostname\n");
        printf("Could not reach %s\n", hn);
        error = 1;
        goto ERROR;
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error setting reference socket\n");
        error = 1;
        goto ERROR;
    }

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Error setting timeout");
        exit(1);
    }

    memset(&their_addr, 0, sizeof(their_addr));
    /* zero struct */
    their_addr.sin_family = AF_INET;
    /* host byte order .. */
    their_addr.sin_port = htons(123);
    /* .. short, netwk byte order */
    their_addr.sin_addr = *((struct in_addr *) he->h_addr);


//get reference time
    get_ntp_time(&tv, &ntp);

//change byte order

    tempSeconds = htonl(ntp.second);
    tempFractions = htonl(ntp.fraction);

    referencePacket.trans_ts_sec = (uint32_t) tempSeconds;
    referencePacket.trans_ts_frac = (uint32_t) tempFractions;

//send packet


    if ((sendto(sockfd, &referencePacket, sizeof(referencePacket), 0,
                (struct sockaddr *) &their_addr, sizeof(struct sockaddr))) == -1) {
        perror("Error sending reference packet\n");
        error = 1;
        goto ERROR;
    }

//set address length
    addr_len = sizeof(struct sockaddr_in);

    memset(&tv, 0, sizeof(struct timeval));

//listen for response
    if ((recvfrom(sockfd, &referencePacket, sizeof(referencePacket), 0,
                  (struct sockaddr *) &their_addr, &addr_len)) == -1) {
        perror("Error receiving reference packet\n");
        error = 1;
        goto ERROR;
    }

//get reference stratum
    stratumTemp = referencePacket.stratum;
//update our stratum to ref+1
    stratumTemp++;
//disable sync warnings
    sendPacket->LI = 0;
//set reference variables
    sendPacket->ref_ts_sec = referencePacket.trans_ts_sec;
    sendPacket->ref_ts_frac = referencePacket.trans_ts_frac;
    sendPacket->ref_ID = referencePacket.ref_ID;
    sendPacket->root_delay = referencePacket.root_delay;
    sendPacket->root_dispersion = referencePacket.root_dispersion;
    sendPacket->precision = referencePacket.precision;
//set stratum
    sendPacket->stratum = stratumTemp;

    printf("Reference update succeeded.\n");

    ERROR:
    if (error == 1) {
        printf("Failed to update reference time\n");
    }
}

#endif //SNTP_COURSEWORK_TIME_CONVERSION_H
