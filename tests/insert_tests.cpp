#include "testing_utilites.hpp"
void do_insert_tests(void)
{
    MyString test_string;
    //index versions
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
    //check if inserting an empty string modifies the object
    test_string.insert(5,"");
    are_equal(test_string.c_str(),"lorem lorem ipsum");
    //iterator versions
    test_string.clear();
    MyString::iterator it=test_string.begin();
    test_string.insert(it,5,'a');
    are_equal(test_string.c_str(),"aaaaa");
    it=test_string.begin();
    for(size_t i=0;i<5;i++)
        it++;
    test_string.insert(it,3,'z');
    are_equal(test_string.c_str(),"aaaaazzz");
    test_string.clear();
    it=test_string.begin();
    test_string.insert(0,"lorem ipsum");
    are_equal(test_string.c_str(),"lorem ipsum");
    it=test_string.begin();
    for(size_t i=0;i<5;i++)
        it++;
    test_string.insert(it," lorem ipsum",6);
    are_equal(test_string.c_str(),"lorem lorem ipsum");
    test_string.clear();
    it=test_string.begin();
    test_string.insert(it,std::string("lorem ipsum"));
    are_equal(test_string.c_str(),"lorem ipsum");
    it=test_string.begin();
    for(size_t i=0;i<5;i++)
        it++;
    test_string.insert(it,std::string(" lorem ipsum"),6);
    are_equal(test_string.c_str(),"lorem lorem ipsum");
    //check if inserting an empty string modifies the object
    it=test_string.begin();
    for(size_t i=0;i<5;i++)
        it++;
    test_string.insert(it,"");
    are_equal(test_string.c_str(),"lorem lorem ipsum");
}