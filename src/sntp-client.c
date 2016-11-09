#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

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


int main()
{

  struct sntpPacket packet;
  memset(&packet, 0, sizeof(struct sntpPacket));

  packet.LI = 0;
  packet.VN = 4;
  packet.mode = 3;

  *((uint8_t*)&packet) = 0x1b;





}
