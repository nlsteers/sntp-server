//
// Created by Nathaniel Steers on 20/11/2016.
//

#ifndef SNTP_COURSEWORK_SNTP_PACKET_H
#define SNTP_COURSEWORK_SNTP_PACKET_H

#include <stdint.h>
#define SNTP_GROUP "224.0.1.1"

//structure for the SNTP packet

struct sntpPacket {
    //-----first 8 bits for flags------
    //reversed from RFC because otherwise it doesn't work...
    unsigned MODE:3;
    unsigned VN:3;
    unsigned LI:2;
    //---------------------------------
    uint8_t stratum;
    uint8_t poll;
    int8_t precision;
    int32_t root_delay;
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

#endif //SNTP_COURSEWORK_SNTP_PACKET_H
