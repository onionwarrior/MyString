%module my_string
%{
#define SWIG_FILE_WITH_INIT
#include "my_string.hpp"
%}
%include "my_string.hpp"
%define __STR__(class_name)
%feature("python:slot","tp_str",functype="reprfunc") class_name::py_to_string;
%extend class_name{
    const char*py_to_string()
    {
        return self->c_str();
    }
}
%enddef
__STR__(MyString);