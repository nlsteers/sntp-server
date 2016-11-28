//
// Created by Nathaniel Steers on 27/11/2016.
//

#include "gtest/gtest.h"

#include "include/time-conversion.h"



TEST(checks, test_log_function){
    //checks that the log2 function returns an expected result (within error bounds)
    double expected = 4.3219280948873626;
    double actual = Log2(20);
    EXPECT_DOUBLE_EQ(actual, expected);
}

TEST(checks, test_sntp_group){
    //checks that the SNTP multicast group is correct
    EXPECT_EQ(SNTP_GROUP, "224.0.1.1");
}

TEST(checks, test_ntp_to_unix){
    //checks that the ntp to unix conversion returns an expected result
    struct timeval tv;
    struct ntp_time_t ntp;
    ntp.second = 3689345377;
    ntp.fraction = 1562367368;
    ntp_time_to_unix_time(&ntp, &tv);
    EXPECT_EQ(tv.tv_sec, 1480356577);
    EXPECT_EQ(tv.tv_usec, 363766);
}

TEST(checks, test_unix_to_ntp){
    //checks that the unix to ntp conversion returns an expected result
    struct timeval tv;
    struct ntp_time_t ntp;
    tv.tv_sec = 1480356577;
    tv.tv_usec = 363766;
    unix_time_to_ntp_time(&tv, &ntp);
    EXPECT_EQ(ntp.second, 3689345377);
    EXPECT_EQ(ntp.fraction, 1562367368);
}

