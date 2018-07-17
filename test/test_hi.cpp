#include "gtest/gtest.h"
#include "hello.h"

TEST(TestHi, Hi)
{
    ASSERT_EQ(say_hi("Travis"), 1);
}
