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
%extend MyString
{
    MyString __add__(const MyString&rhs)
    {
        return operator+(*$self,rhs);
    }
    bool __eq__(const MyString&rhs)
    {
        return operator==(*$self,rhs);
    }
    bool __ne__(const MyString&rhs)
    {
        return operator!=(*$self,rhs);
    }
    bool __eq__(const char*rhs)
    {
        return operator==(*$self,rhs);
    }
    bool __ne__(const char*rhs)
    {
        return operator!=(*$self,rhs);
    }
    MyString __add__(const char *rhs)
    {
        return operator+(*$self,rhs);
    }
    bool __lt__(const MyString&rhs)
    {
        return operator<(*$self,rhs);
    }
    bool __le__(const MyString&rhs)
    {
        return operator<=(*$self,rhs);
    }
    bool __gt__(const MyString&rhs)
    {
        return operator>(*$self,rhs);
    }
    bool __ge__(const MyString&rhs)
    {
        return operator>=(*$self,rhs);
    }
    char __getitem__(unsigned i)
	{	
        return (*$self)[i];
    }
	void __setitem__(unsigned i,char v)
	{	(*$self)[i]=v;	}
    
}
%ignore MyString::MyString(std::initializer_list<char>);
%rename(__setitem__) MyString::__setitem__;
%rename(__getitem__) MyString::__getitem__;
__STR__(MyString);