#include "testing_utilites.hpp"
void do_find_tests()
{
    MyString test_string("lorem ipsum");
    assert(test_string.find("lorem ipsum")==0);
    assert(test_string.find("ipsum lorem")==-1);
    assert(test_string.find("i")==6);
    assert(test_string.find("m")==4);
    assert(test_string.find("m",5)==10);
    assert(test_string.find("lorem")==0);
    assert(test_string.find("rem")==2);
    assert(test_string.find("sum")==8);
}