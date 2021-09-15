#include "MyString.hpp"
#include <iostream>
#include <iosfwd>
#include <fstream>
#include <algorithm>
template<typename T>
void ab(T arg)
{
   std::cout<<std::is_pointer<std::decay_t<T> >::value;
}

int main()
{
   MyString a1("Hello world!");
   a1.append("abob");
std::string s1;
s1 = a1.data();
std::cout<<s1+"aaa";
std::cout<<"aaa"+s1;
std::cout<<s1<<std::endl;

   try
   {  
      MyString a=547;
      std::cout<<a;
      std::cout<<a.capacity();
   }
   catch(const std::invalid_argument&e)
   {
      std::cout<<e.what();
   }
}
//ndefined reference to `std::basic_ostream<char, std::char_traits<char> >& operator<< <char>(std::basic_ostream<char, std::char_traits<char> >&, MyString const&)'