#include "testing_utilites.hpp"
void do_insert_tests(void)
{
    MyString test_string;
    test_string.insert(0,5,'a');
    are_equal(test_string.c_str(),"aaaaa");
    test_string.insert(5,3,'z');
    are_equal(test_string.c_str(),"aaaaazzz");
    test_string.clear();
    test_string.insert(0,"lorem ipsum");
    are_equal(test_string.c_str(),"lorem ipsum");
    test_string.insert(5," lorem ipsum",6);
    are_equal(test_string.c_str(),"lorem lorem ipsum");
    test_string.clear();
    test_string.insert(0,std::string("lorem ipsum"));
    are_equal(test_string.c_str(),"lorem ipsum");
    test_string.insert(5,std::string(" lorem ipsum"),6);
    are_equal(test_string.c_str(),"lorem lorem ipsum");
}