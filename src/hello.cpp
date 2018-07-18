#include <cstdio>
#include "hello.h"

int say_hello(const char* const name) noexcept
{
    std::printf("Hello, %s!\n", name);
    return 1;
}

int say_hi(const char* const name) noexcept
{
    std::printf("Hi, %s!\n", name);
    return 1;
}
