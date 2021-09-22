#include <boost/python.hpp>
//#include <boost/python/detail/wrap_python.hpp>
#include "my_string.hpp"
using namespace boost::python;
BOOST_PYTHON_MODULE(mystring)
{
    class_<MyString>("MyString")
        .def("c_str", &MyString::c_str,return_value_policy<return_by_value>())
        .def(init<const char*,size_t>(args("c_str","count")));
}
