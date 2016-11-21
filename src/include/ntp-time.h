//
// Created by Nathaniel Steers on 20/11/2016.
//

#ifndef SNTP_COURSEWORK_NTP_TIME_H
#define SNTP_COURSEWORK_NTP_TIME_H

#include <ntsid.h>
#include <stdint.h>

struct ntp_time_t {
    __darwin_time_t second;
    uint32_t   fraction;
};

#endif //SNTP_COURSEWORK_NTP_TIME_H
