//
// Created by Nathaniel Steers on 27/11/2016.
//

#include "gtest/gtest.h"

#include "include/time-conversion.h"

TEST(checks, test_eq){
    EXPECT_EQ(1, 1);
}

TEST(checks, test_neq) {
    EXPECT_NE(1, 0);
}

TEST(checks, test_log_function){
    double expected = 4.3219280948873626;
    double actual = Log2(20);
    EXPECT_DOUBLE_EQ(actual, expected);
}

TEST(checks, test_sntp_group){
    EXPECT_EQ(SNTP_GROUP, "224.0.1.1");
}