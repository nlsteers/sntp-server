#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/time-conversion.h"

uint16_t PORT = 4950;


int main(int argc, char *argv[]) {
    int sockfd, i, multicast;
    struct sntpPacket sendPacket, recPacket;
    struct ntp_time_t ntp = {0};
    struct timeval tv = {0};
    struct sockaddr_in my_addr;
    /* info for my addr i.e. server */
    struct sockaddr_in their_addr;
    /* client's address info */
    int addr_len, quit, update, error, ttl;
    long tempSeconds, tempFractions;
    struct ip_mreq multi;

    u_int y = 1;
    quit = 0;
    update = 1;
    error = 0;
    ttl = 64;
    multicast = 0;

    //zero packets
    memset(&sendPacket, 0, sizeof(struct sntpPacket));
    memset(&recPacket, 0, sizeof(struct sntpPacket));

    //iterate through command line arguments
    for (i = 1; i < argc; i++) {
        //show help
        if (strcmp(argv[i], "-help") == 0) {
            printf("Usage: \n\n"
                           "-multicast\n"
                           "Enables listening on a multicast group\n\n"
                           "-port\n"
                           "Use requested port\n\n");
            exit(0);
        }
        //enable multicast
        if (strcmp(argv[i], "-multicast") == 0) {
            printf("Listening on multicast channel %s\n", SNTP_GROUP);
            multicast = 1;
        }

        //change port
        if (strcmp(argv[i], "-port") == 0) {

            unsigned short int newPort;
            int inputs;

            printf("Enter port number: ");
            inputs = scanf("%hu", &newPort);
            getchar();
            if (inputs == EOF) {
                printf("Bad input, exiting...");
                exit(1);
            } else if (inputs == 0) {
                printf("Using default port: %d\n", PORT);
            } else {
                PORT = newPort;
            }

        }

    }


    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket error");
        exit(1);
    }

    if (multicast == 1) {

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y)) < 0) {
            perror("Socket reuse error");
            exit(1);
        }

        if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
            perror("Socket ttl error");
            exit(1);
        }

        memset(&my_addr, 0, sizeof(my_addr));
        /* zero struct */
        my_addr.sin_family = AF_INET;
        /* host byte order ... */

        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        my_addr.sin_port = htons(PORT);
        /* ... short, network byte order */

        if (bind(sockfd, (struct sockaddr *) &my_addr,
                 sizeof(struct sockaddr)) == -1) {
            perror("Socket bind error");
            exit(1);
        }

        multi.imr_multiaddr.s_addr = inet_addr(SNTP_GROUP);
        multi.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multi, sizeof(multi)) < 0) {
            perror("Error joining multicast group");
            exit(1);
        }

        printf("Joined multicast group %s ...\n", SNTP_GROUP);
    }

    if (multicast == 0){

        memset(&my_addr, 0, sizeof(my_addr));
        /* zero struct */
        my_addr.sin_family = AF_INET;
        /* host byte order ... */
        my_addr.sin_port = htons(PORT);
        /* ... short, network byte order */
        my_addr.sin_addr.s_addr = INADDR_ANY;
        /* any of server IPs */


        if (bind(sockfd, (struct sockaddr *) &my_addr,
                 sizeof(struct sockaddr)) == -1) {
            perror("Socket bind error");
            exit(1);
        }
    }

    addr_len = sizeof(struct sockaddr);

    //clock not synchronized
    sendPacket.LI = 3;
    //version 4
    sendPacket.VN = 4;
    //server response
    sendPacket.MODE = 4;

    sendPacket.stratum = 1;

    do {

//listen for request

        //update ref time
        if (update == 1) {
            get_reference_time(&sendPacket);
            update = 0;
            printf("Listening...\n");
        }

        if ((recvfrom(sockfd, &recPacket, sizeof(recPacket), 0,
                      (struct sockaddr *) &their_addr, (socklen_t *) &addr_len)) == -1) {
            perror("Bad request");
            error = 1;
            goto ERROR;
        }
        get_ntp_time(&tv, &ntp);

        printf("Got request from %s\n", inet_ntoa(their_addr.sin_addr));

//check that the client did not supply a poll time
        if (recPacket.poll == 0) {
            sendPacket.poll = (uint8_t) Log2(20);
        } else {
            sendPacket.poll = recPacket.poll;
        }
//set response origin time to transmit time of request
        sendPacket.origin_ts_sec = recPacket.trans_ts_sec;
        sendPacket.origin_ts_frac = recPacket.trans_ts_frac;
//host to network byte swap
        tempSeconds = htonl(ntp.second);
        tempFractions = htonl(ntp.fraction);

        sendPacket.recv_ts_sec = (uint32_t) tempSeconds;
        sendPacket.recv_ts_frac = (uint32_t) tempFractions;

        memset(&tv, 0, sizeof(struct timeval));
        memset(&ntp, 0, sizeof(struct ntp_time_t));
//get system time
        get_ntp_time(&tv, &ntp);

        tempSeconds = htonl(ntp.second);
        tempFractions = htonl(ntp.fraction);
//set transmitted time of response
        sendPacket.trans_ts_sec = (uint32_t) tempSeconds;
        sendPacket.trans_ts_frac = (uint32_t) tempFractions;

        if ((sendto(sockfd, &sendPacket, sizeof(sendPacket), 0,
                    (struct sockaddr *) &their_addr, sizeof(struct sockaddr))) == -1) {
            perror("Could not send response");
            error = 1;
            goto ERROR;

        }
//check the reference time, if 60 seconds out of date since last response update
        if (ntohl(sendPacket.trans_ts_sec) > (ntohl(sendPacket.ref_ts_sec) + 60)) {
            update = 1;
        }

        memset(&tv, 0, sizeof(struct timeval));
        memset(&ntp, 0, sizeof(struct ntp_time_t));

        printf("Response sent to %s\n", inet_ntoa(their_addr.sin_addr));
//label to skip method if errors encountered
        ERROR:
        if (error == 1) {
            printf("Response not formed\n");
            error = 0;
        }


    } while (quit != 1);
    
    //should never get here

    printf("Exiting...\n");
    close(sockfd);
    exit(0);
}
