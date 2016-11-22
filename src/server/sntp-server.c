#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/time-conversion.h"

#define MYPORT 4950
/* the port users connect to */


int main(void) {
    int sockfd;
    struct sntpPacket sendPacket, recPacket;
    struct ntp_time_t ntp, originntp, transmitntp, receiventp = {0};
    struct timeval tv, arrival, origintv, transmittv, receivetv = {0};
    struct sockaddr_in my_addr;
    /* info for my addr i.e. server */
    struct sockaddr_in their_addr;
    /* client's address info */
    int addr_len, numbytes, quit;
    quit  = 1;

    memset(&sendPacket, 0, sizeof(struct sntpPacket));
    memset(&recPacket, 0, sizeof(struct sntpPacket));

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Listener socket");
        exit(1);
    }
    memset(&my_addr, 0, sizeof(my_addr));
    /* zero struct */
    my_addr.sin_family = AF_INET;
    /* host byte order ... */
    my_addr.sin_port = htons(MYPORT);
    /* ... short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY;
    /* any of server IP addrs */
    if (bind(sockfd, (struct sockaddr *) &my_addr,
             sizeof(struct sockaddr)) == -1) {
        perror("Socket bind error");
        exit(1);
    }
    addr_len = sizeof(struct sockaddr);

    printf("Listening...\n");


    do {

//listen for response

        if ((recvfrom(sockfd, &recPacket, sizeof(recPacket), 0,
                      (struct sockaddr *) &their_addr, (socklen_t *) &addr_len)) == -1) {
            perror("SNTP recvfrom error");
            quit = 0;
        }
        get_ntp_time(&tv, &ntp);

        printf("Got packet from %s\n", inet_ntoa(their_addr.sin_addr));
        print_network_packet(&recPacket);

        sendPacket.flags = 0x24;
        sendPacket.stratum = 0;
        sendPacket.poll = 0;
        sendPacket.precision = 0;

        sendPacket.origin_ts_sec = recPacket.trans_ts_sec;
        sendPacket.origin_ts_frac = recPacket.trans_ts_frac;

        long tempSeconds = htonl(ntp.second);
        long tempFractions = htonl(ntp.fraction);

        sendPacket.recv_ts_sec = (uint32_t) tempSeconds;
        sendPacket.recv_ts_frac = (uint32_t) tempFractions;

        memset(&tv, 0, sizeof(struct timeval));
        memset(&ntp, 0, sizeof(struct ntp_time_t));

        get_ntp_time(&tv, &ntp);

        tempSeconds = htonl(ntp.second);
        tempFractions = htonl(ntp.fraction);

        sendPacket.trans_ts_sec = (uint32_t) tempSeconds;
        sendPacket.trans_ts_frac = (uint32_t) tempFractions;


        printf("Packet to send: \n");
        print_network_packet(&sendPacket);

//change byte order
/*
        long tempSeconds = htonl(sendPacket.trans_ts_sec);
        long tempFractions = htonl(sendPacket.trans_ts_frac);

        sendPacket.trans_ts_sec = (uint32_t) tempSeconds;
        sendPacket.trans_ts_frac = (uint32_t) tempFractions;

*/



    } while (quit != 0);
    printf("Exiting...\n");
    close(sockfd);
    exit(0);
}
