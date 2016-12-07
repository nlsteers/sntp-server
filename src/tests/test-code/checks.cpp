//
// Created by Nathaniel Steers on 27/11/2016.
// Unit tests use the googletest c unit testing library
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

TEST(checks, test_get_ref_code){
    //checks that the reference code can be determined
    struct sntpPacket testpacket;
    memset(&testpacket, 0, sizeof(struct sntpPacket));
    memcpy(&testpacket.ref_ID, "DROP", 4);
    int code = getCode(&testpacket);
    EXPECT_EQ(code, 16);
}

TEST(checks, test_calc_offset){

    struct timeval arrival, origin, transmit, receive = {0};

    arrival.tv_sec = 1481127684;
    arrival.tv_usec = 54265;
    origin.tv_sec = 1481127684;
    origin.tv_usec = 32412;
    transmit.tv_sec = 1481127684;
    transmit.tv_usec = 37791;
    receive.tv_sec = 1481127684;
    receive.tv_usec = 37685;


    double oTotal = calcOffset(&arrival, &origin, &transmit, &receive);
    double expected = -0.0029640000000000001;

    EXPECT_DOUBLE_EQ(oTotal, expected);

}

TEST(checks, test_calc_delay){

    struct timeval arrival, origin, transmit, receive = {0};

    arrival.tv_sec = 1481127684;
    arrival.tv_usec = 54265;
    origin.tv_sec = 1481127684;
    origin.tv_usec = 32412;
    transmit.tv_sec = 1481127684;
    transmit.tv_usec = 37791;
    receive.tv_sec = 1481127684;
    receive.tv_usec = 37685;

    double dTotal = calcDelay(&arrival, &origin, &transmit, &receive);
    double expected = 0.021746999999999999;

    EXPECT_DOUBLE_EQ(dTotal, expected);

}
