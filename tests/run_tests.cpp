#include "constructor_tests.cpp"
#include "append_tests.cpp"
int main()
{
    do_constructor_tests();
    do_append_tests();
    MyString a="aboba";
    MyString b="bebra"+a;
    std::cout<<a+b;
}