#include <cstdio>
#include "hello.h"

int say_hello(const char* const name) noexcept
{
    std::printf("Hello, %s!", name);
    return 1;
}

int say_hi(const char* const name) noexcept
{
    std::printf("Hi, %s! ", name);
    return 1;
}
