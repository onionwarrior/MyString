#include "testing_utilites.hpp"
void do_to_tests()
{
    MyString unsigned_integer("123456789");
    assert(unsigned_integer.to<int>()==123456789);
    assert(unsigned_integer.to<unsigned int>()==123456789);
    MyString signed_integer("-123456789");
    assert(signed_integer.to<int>()==-123456789);
    MyString float_string("3.0007");
    assert(float_string.to<float>()==3.0007f);
    assert(float_string.to<double>()==3.0007);
    bool did_throw=false;
    MyString test_string("should throw");
    try
    {
        test_string.to<int>();
    }
    catch(const MyString::InvalidConversion& e)
    {
        did_throw=true;
    }
    assert(did_throw);
    did_throw=false;
    try
    {
        test_string.to<float>();
    }
    catch(const MyString::InvalidConversion& e)
    {
        did_throw=true;
    }
    assert(did_throw);
}