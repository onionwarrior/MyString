#include "testing_utilites.hpp"
void do_operator_tests(void)
{
    MyString small="abc small";
    MyString large="def large";
    assert(small<large);
    assert(large>small);
    assert(small<=large);
    assert(large>=small);
    assert(small=="abc small");
    assert(small==MyString("abc small"));
    MyString sum;
    sum+=small;
    assert(sum==small);
    sum+=large;
    assert(sum==small+large);
}