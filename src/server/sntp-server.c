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
    struct sockaddr_in my_addr;
    /* info for my addr i.e. server */
    struct sockaddr_in their_addr;
    /* client's address info */
    int addr_len, numbytes;
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

    do {

//listen for response

        if ((recvfrom(sockfd, &recPacket, sizeof(recPacket), 0,
                      (struct sockaddr *) &their_addr, (socklen_t *) &addr_len)) == -1) {
            perror("SNTP recvfrom error");
            exit(1);
        }

        printf("Got packet from %s\n", inet_ntoa(their_addr.sin_addr));
        print_network_packet(&recPacket);

    } while (strcmp(buf, "quit") != 0);
    printf("Exiting...\n");
    close(sockfd);
    exit(0);
}
