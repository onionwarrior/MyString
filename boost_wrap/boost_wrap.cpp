#include <boost/python.hpp>
#include "../include/my_string.hpp"
using namespace boost::python;
std::string __str__(const MyString &str)
{
    return str.c_str();
}
char __getitem__(MyString&f,int index)
{
    return f[index];
}
void __setitem__(MyString&f,int index,char v)
{
    f[index]=v;
}
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(insert_overloads,insert , 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( append_overloads,append,1,3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( substr_overloads,substr,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS( find_overloads,find,1,2)
BOOST_PYTHON_MODULE(my_string_boost)
{
    class_<MyString>("MyString")
        .def("length",&MyString::length)
        .def("size",&MyString::size)
        .def("empty",&MyString::empty)
        .def("capacity",&MyString::capacity)
        .def("shrink_to_fit",&MyString::shrink_to_fit)
        .def("clear",&MyString::clear)
        // this doesnt work smh .def(init<const char *, std::optional<size_t> >())
        .def(init<const char *>())
        .def(init<const char *,size_t>())
        .def(init<int>())
        .def(init<float>())
        //this causes some weird calls to wrong constructor(python does not have a char type)
        //.def(init<char,size_t>())
        .def(init<const MyString&>())
        .def(init<const std::string&>())\
        .def("__str__", __str__)
        .def("__getitem__",&__getitem__)
        .def("__setitem__",&__setitem__)
        .def("insert",static_cast<void (MyString::*)(size_t,const std::string&,size_t)>(&MyString::insert),insert_overloads())
        .def("append",static_cast<void (MyString::*)(const std::string&,size_t,size_t)>(&MyString::append),append_overloads())
        .def("replace",static_cast<void (MyString::*)(size_t,size_t,const std::string&)>(&MyString::replace))
        .def("substr",static_cast<MyString (MyString::*)(size_t,size_t) const>(&MyString::substr),substr_overloads())
        .def("erase",static_cast<void (MyString::*)(size_t,size_t)>(&MyString::erase))
        .def("find",static_cast<int (MyString::*)(const std::string&,size_t) const>(&MyString::find),find_overloads())
        .def(self+self)
        .def(self<self)
        .def(self==self)
        .def(self!=self)
        .def(self<=self)
        .def(self>self)
        .def(self>=self);
}
