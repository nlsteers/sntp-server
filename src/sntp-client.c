#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include "include/timeconversion.c"

#define PORT 123
#define MAXBUFLEN 100

struct sntpPacket {
  unsigned int LI : 2;
  unsigned int VN : 3;
  unsigned int mode : 3;
	uint8_t stratum;
	uint8_t poll;
	uint8_t precision;
	uint32_t root_delay;
	uint32_t root_dispersion;
	uint32_t ref_ID;
	uint32_t ref_ts_sec;
	uint32_t ref_ts_frac;
	uint32_t origin_ts_sec;
	uint32_t origin_ts_frac;
	uint32_t recv_ts_sec;
	uint32_t recv_ts_frac;
	uint32_t trans_ts_sec;
	uint32_t trans_ts_frac;
};


int main(int argc, char *argv[])
{

  struct sntpPacket packet;
  memset(&packet, 0, sizeof(struct sntpPacket));

  packet.LI = 0;
  packet.VN = 4;
  packet.mode = 3;

  *((uint8_t*)&packet) = 0x1b;

  int sockfd, numbytes;

  socklen_t addr_len;
  struct hostent *he;
  struct sockaddr_in their_addr;
  int c;
  int count;
  char hn[256];
  count = 0;

  printf("Enter hostname: ");
  for(count = 0; (c = getchar()) != '\n' && count < 256; count++) {
      hn[count] = c;
  }
  hn[count] = '\0';


  if( (he = gethostbyname( hn)) == NULL) {
    perror( "SNTP gethostbyname error");
    exit( 1);
  }
  if( (sockfd = socket( AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror( "SNTP socket error");
    exit( 1);
  }

  memset( &their_addr,0, sizeof(their_addr));
  /* zero struct */
  their_addr.sin_family = AF_INET;
  /* host byte order .. */
  their_addr.sin_port = htons( PORT);
  /* .. short, netwk byte order */
  their_addr.sin_addr = *((struct in_addr *)he -> h_addr);

  //change byte order
  packet.ref_ts_sec = htonl(packet.ref_ts_sec);
  packet.ref_ts_frac = htonl(packet.ref_ts_frac);
  packet.origin_ts_sec = htonl(packet.origin_ts_sec);
  packet.origin_ts_frac = htonl(packet.origin_ts_frac);
  packet.recv_ts_sec = htonl(packet.recv_ts_sec);
  packet.recv_ts_frac = htonl(packet.recv_ts_frac);

//send packet
  if( (numbytes = sendto( sockfd, &packet, sizeof( packet), 0,
  (struct sockaddr *)&their_addr, sizeof( struct sockaddr))) == -1) {
    perror( "SNTP sendto error");
    exit( 1);
  }
  printf( "Sent %d bytes to %s\n", numbytes, inet_ntoa( their_addr.sin_addr));


//set address length
  addr_len = sizeof(struct sockaddr_in);

//listen for response
  if( (numbytes = recvfrom( sockfd, &packet, sizeof( packet), 0,
  (struct sockaddr *)&their_addr, &addr_len)) == -1) {
    perror( "Listener recvfrom");
    exit( 1);
  }


//print ntp time
  printf( "NTP seconds recv: %u \nNTP fractions recv: %u \n", ntohl(packet.recv_ts_sec), ntohl(packet.recv_ts_frac));

  struct ntp_time_t ntp;
  struct timeval tv = {0};

  ntp.second = ntohl(packet.recv_ts_sec);
  ntp.fraction = ntohl(packet.recv_ts_frac);

//convert to UNIX time
  ntp_time_to_unix_time(&ntp, &tv);
  printf("UNIX Time: %ld %ld\n", tv.tv_sec, tv.tv_usec);

  unsigned long long millisecondsSinceEpoch = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
//print time in milliseconds
  printf("Milliseconds since epoch (UNIX) %llu\n", millisecondsSinceEpoch);


//convert to human readable
  print_unix_to_hr(&tv);

  close( sockfd);
  return 0;
}
