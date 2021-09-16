#include "testing_utilites.hpp"
void do_substr_tests()
{
    MyString test_string;
    are_equal(test_string.substr(0,0).c_str(),"");
    test_string="lorem ipsum";
    are_equal(test_string.substr(0,5).c_str(),"lorem");
    are_equal(test_string.substr(6).c_str(),"ipsum");
    MyString::iterator it=test_string.begin();
    are_equal(test_string.substr(it,5).c_str(),"lorem");
    for(size_t i=0;i<6;i++)
        it++;
    are_equal(test_string.substr(it).c_str(),"ipsum");
}