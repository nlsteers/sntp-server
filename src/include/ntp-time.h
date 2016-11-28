//
// Created by Nathaniel Steers on 20/11/2016.
//

#ifndef SNTP_COURSEWORK_NTP_TIME_H
#define SNTP_COURSEWORK_NTP_TIME_H

#include <stdint.h>

//ntp time structure

struct ntp_time_t {
    uint32_t second;
    uint32_t fraction;
};

#endif //SNTP_COURSEWORK_NTP_TIME_H
