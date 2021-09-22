%module my_string_swig
%{
#define SWIG_FILE_WITH_INIT
#include "../include/my_string.hpp"
%}
%include "../include/my_string.hpp"
%define __STR__(class_name)
%feature("python:slot","tp_str",functype="reprfunc") class_name::py_to_string;
%extend class_name{
    const char*py_to_string()
    {
        return self->c_str();
    }
}
%enddef
%ignore MyString::MyString(std::initializer_list<char>);
%rename(add) MyString::operator+(const MyString &,const MyString &);
%rename(eq) MyString::operator==(const MyString &,const MyString &);
__STR__(MyString);