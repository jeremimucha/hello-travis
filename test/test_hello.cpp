#include "gtest/gtest.h"
#include "hello.h"

TEST(HelloTest, Hello)
{
    ASSERT_EQ(say_hello("Test"), 1);
}
