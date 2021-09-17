#pragma once
#include <assert.h>
#include <cstring>
#include "../my_string.hpp"
void are_equal(const char *a,const char*b)
{
    assert(strcmp(a,b)==0);
}