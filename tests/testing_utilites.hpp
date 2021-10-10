#ifndef TESTING_UTIL_H
#define TESTING_UTIL_H
#include <assert.h>
#include <cstring>
#include "../include/my_string.hpp"
void are_equal(const char *a,const char*b)
{
    assert(strcmp(a,b)==0);
}
#endif