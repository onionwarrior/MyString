#include "boost/python/detail/wrap_python.hpp"
#include "my_string.hpp"
#include <iostream>
#include <iosfwd>
#include <fstream>
#include <algorithm>
#include <vector>
template<typename T>
void ab(T arg)
{
   std::cout<<std::is_pointer<std::decay_t<T> >::value;
}
int main()
{
   MyString a1("Hello world!");
   a1.append("abob");
   //a1.insert(a1.rend(),"aaa");
   std::string z("za");
   z.insert(0,"aaa",3);
   std::cout<<z;
   std::string s1;
   s1 = a1.data();
   std::cout<<s1+"aaa";
   std::cout<<"aaa"+s1;
   std::cout<<s1<<std::endl;
   MyString test;
   std::ifstream in("./a.txt");
   while(!in.eof())
      in>>test;
   std::cout<<test;
   try
   {  
      MyString a(547);
      std::cout<<a;
      std::cout<<a.capacity()<<'\n';
      MyString test(0.05);
      std::cout<<test;
      //std::cout<<std::vector<int>();
      //std::cout<<""<<b.c_str()<<"";
   }
   catch(const std::invalid_argument&e)
   {
      std::cout<<e.what();
   }
}
