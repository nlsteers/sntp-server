//
// Created by Nathaniel Steers on 27/11/2016.
//

#include "gtest/gtest.h"

TEST(checks, test_eq){
    EXPECT_EQ(1, 0);
}

TEST(checks, test_neq) {
    EXPECT_NE(1, 0);
}
