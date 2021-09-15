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
   MyString a1 = "aaaaa";
a1.insert(0,1,'!');
std::cout<<a1<<std::endl;
a1.insert(3, 2, '@');
std::cout << a1 << std::endl;
a1.insert(a1.end(),4,'z');
std::cout << a1 << std::endl;
a1.insert(a1.begin(),std::string("aboba"));
std::cout << a1 << std::endl;
a1.insert(a1.rend().base(),"aboba");
std::string a("zzzz");
a1.insert(a1.rend().base(),a);
std::cout << a1 << std::endl;
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