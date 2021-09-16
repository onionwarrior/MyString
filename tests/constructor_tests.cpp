#include "testing_utilites.hpp"
void do_constructor_tests()
{
    //default constructor test
    MyString empty_string;
    assert(strcmp(empty_string.c_str(),"")==0);
    //c-string test
    const char * test_cstr="lorem ipsum dolor sit amet";
    const char * test_cstr_large=
    "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\
    ";
    MyString string_from_cstr(test_cstr);
    are_equal(string_from_cstr.c_str(),test_cstr);
    MyString string_from_large_cstr(test_cstr_large);
    are_equal(string_from_large_cstr.c_str(),test_cstr_large);
    std::initializer_list init_list{'l', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l', 'o', 'r', ' ', 's', 'i', 't', ' ', 'a', 'm', 'e', 't'};
    MyString string_from_init_list(init_list);
    are_equal(string_from_init_list.c_str(),"lorem ipsum dolor sit amet");
    std::string str_lvalue(test_cstr);
    MyString string_from_string_lvalue{std::string(test_cstr)}, string_from_string_rvalue{str_lvalue};
    are_equal(string_from_string_lvalue.c_str(),test_cstr);
    are_equal(string_from_string_rvalue.c_str(),test_cstr);
    MyString string_copy{string_from_cstr},string_move{std::move(string_from_string_lvalue)};
    are_equal(string_copy.c_str(),test_cstr);
    are_equal(string_move.c_str(),test_cstr);
    //did we really move?
    assert(string_from_string_lvalue.c_str()==nullptr);
    MyString count_char_string('a',10);
    are_equal(count_char_string.c_str(),"aaaaaaaaaa");
    MyString count_str_string("lorem ipsum?",10);
    //could be more elegant
    are_equal(count_str_string.c_str(),"lorem ipsum?lorem ipsum?lorem ipsum?lorem ipsum?lorem ipsum?lorem ipsum?lorem ipsum?lorem ipsum?lorem ipsum?lorem ipsum?");
    const char * exceptionally_large_cstr=
    #include "exceptionally_large_string.txt"
    ;
    MyString exceptionally_large(exceptionally_large_cstr);
    are_equal(exceptionally_large_cstr,exceptionally_large.c_str());
    MyString from_sint_pos(123456789),from_sint_neg(-123456789);
    are_equal(from_sint_pos.c_str(),"123456789");
    are_equal(from_sint_neg.c_str(),"-123456789");
    MyString from_uint(1234567890u);
    are_equal(from_uint.c_str(),"1234567890");
    //weird floats
    MyString from_float(0.123456789f),from_double(0.123456789);
    are_equal(from_float.c_str(),"0.123457");
    are_equal(from_double.c_str(),"0.123457");
}