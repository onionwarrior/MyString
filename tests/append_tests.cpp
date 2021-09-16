#include "../MyString.hpp"
#include "testing_utilites.hpp"
void do_append_tests()
{
    MyString test_string;
    test_string.append(0,'c');
    are_equal(test_string.c_str(),"");
    test_string.append(5,'c');
    are_equal(test_string.c_str(),"ccccc");
    test_string.append(3,'l');
    are_equal(test_string.c_str(),"ccccclll");
    test_string.clear();
    test_string.append("lorem ");
    test_string.append("ipsum");
    are_equal(test_string.c_str(),"lorem ipsum");
    test_string.clear();
    test_string.append("lorem ipsum",0,5);
    test_string.append("lorem ipsum",5,6);
    are_equal(test_string.c_str(),"lorem ipsum");
    test_string.clear();
    std::string lorem{"lorem "},ipsum{"ipsum"},lorem_ipsum{"lorem ipsum"};
    test_string.append(lorem);
    test_string.append(ipsum);
    are_equal(test_string.c_str(),"lorem ipsum");
    test_string.clear();
    test_string.append(lorem_ipsum,0,5);
    test_string.append(lorem_ipsum,5,6);
    are_equal(test_string.c_str(),"lorem ipsum");
    test_string.clear();
}