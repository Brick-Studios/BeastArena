#include <iostream>
#include <gtest/gtest.h>

#include "controllers/game_controller.hpp"

TEST(TEST1, Subtest_1){
    ASSERT_TRUE(1==1);
}

TEST(TEST1, Subtest_2){
    ASSERT_TRUE(2==2);
}

TEST(GAMECONTROLLER, Test1){
    //auto gc = GameController();
    ASSERT_TRUE(1==1);
}