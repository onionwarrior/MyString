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
   MyString zzz(std::move(MyString("aboba")));
   std::fstream a("zzz.txt");
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