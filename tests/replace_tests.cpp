#include "testing_utilites.hpp"
void do_replace_tests(void)
{
    //index versions
    MyString test_string;
    //same as insert at index 0
    test_string.replace(0,0,"lorem ipsum");
    are_equal(test_string.c_str(),"lorem ipsum");
    //replace lorem with ipsum
    test_string.replace(0,5,"ipsum");
    are_equal(test_string.c_str(),"ipsum ipsum");
    //replace second ipsum with lorem
    test_string.replace(6,5,"lorem");
    are_equal(test_string.c_str(),"ipsum lorem");
    //should turn ip_sum lorem into ip_-address
    test_string.replace(2,9,"-address");
    are_equal(test_string.c_str(),"ip-address");
    //replace everything with an empty string, therefore erasing the string
    test_string.replace(0,test_string.size(),"");
    are_equal(test_string.c_str(),"");
    assert(test_string.size()==0);
    test_string.shrink_to_fit();
    assert(test_string.capacity()==1);
    //iterator versions
    test_string.replace(test_string.begin(),0,"lorem ipsum");
    are_equal(test_string.c_str(),"lorem ipsum");
    //replace lorem with ipsum
    test_string.replace(0,5,"ipsum");
    are_equal(test_string.c_str(),"ipsum ipsum");
    //replace second ipsum with lorem
    //const and non-const iterators dont have internal differences so there is no pointing in testing both of them
    //reverse iterators cant be used with this funcs anyway, so no need to test them
    MyString::const_iterator it=test_string.cbegin();
    for(size_t i=0;i<6;i++)
        it++;
    test_string.replace(it,5,"lorem");
    are_equal(test_string.c_str(),"ipsum lorem");
    //should turn ip_sum lorem into ip_-address
    it=test_string.cbegin();
    for(size_t i=0;i<2;i++)
        it++;
    test_string.replace(it,9,"-address");
    are_equal(test_string.c_str(),"ip-address");
    //replace everything with an empty string, therefore erasing the string
    test_string.replace(test_string.begin(),test_string.size(),"");
    are_equal(test_string.c_str(),"");
    assert(test_string.size()==0);
    test_string.shrink_to_fit();
    assert(test_string.capacity()==1);
}