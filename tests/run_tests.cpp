#include "constructor_tests.cpp"
#include "append_tests.cpp"
#include "replace_tests.cpp"
#include "insert_tests.cpp"
#include "substr_tests.cpp"
#include "find_tests.cpp"
int main()
{
    do_constructor_tests();
    do_append_tests();
    do_replace_tests();
    do_insert_tests();
    do_substr_tests();
    do_find_tests();
}