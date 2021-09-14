#include "MyString.hpp"
#include <cstring>
#include <stdexcept>
//Default constructor
MyString::MyString():
    str_(new char[capacity_]())
{}
//C-style string constructor
MyString::MyString(const char * c_str):
    size_(c_str ? std::strlen(c_str) : 0),
    capacity_(size_+1),
    str_(new char[capacity_]())
{
    if(nullptr==c_str)//implement own nullptr exception
        throw std::invalid_argument("nullptr passed to C-tor");
    std::memcpy(str_,c_str,size_);
}
//std::string constructor
MyString::MyString(const std::string& arg):
    MyString(arg.c_str())
{}
//destructor
MyString::MyString(std::initializer_list<char> init_list):
    size_(init_list.size()),
    capacity_(size_+1),
    str_(new char[capacity_]())
{
    size_t i=0;
    for(const auto elem:init_list)
        str_[i++]=elem;
}
//count string constructor
MyString::MyString(const char * c_str,size_t count):
    size_(c_str ? std::strlen(c_str) : 0),
    capacity_(size_+1),
    str_(new char[capacity_]())
{
    if(nullptr==c_str)
        throw std::invalid_argument("nullptr passed to C-tor");
    const auto len=std::strlen(c_str);
    for(size_t i=0;i<count;i++)
        std::memcpy(str_+len*i,c_str,len);
}
//count char constructor
MyString::MyString(char c,size_t count):
    size_(count),
    capacity_(size_+1),
    str_(new char[capacity_]())
{
    std::memset(str_,c,size_);
}
//Copy constructor
MyString::MyString(const MyString&copy):
    size_(copy.size_),
    capacity_(copy.capacity_),
    str_{new char[capacity_]()}
{
    std::memcpy(str_,copy.str_,size_);
}
//Move constructor 
MyString::MyString(MyString&&rhs) noexcept:
    size_{std::exchange(rhs.size_,0)},
    capacity_{std::exchange(rhs.capacity_,0)},
    str_{std::exchange(rhs.str_,nullptr)}
{
}
char * MyString::c_str() const
{
    return str_;
}
char * MyString::data() const
{
    return str_;
}
size_t MyString::size() const
{
    return size_;
}
size_t MyString::length() const
{
    return size_;
}
size_t MyString::capacity() const
{
    return capacity_;
}
void MyString::clear()
{
    std::memset(str_,0,size_);
    size_=0;
}
bool MyString::empty() const
{
    return size_==0;
}
void MyString::resize(size_t new_size)
{
    if(size_<new_size)
        capacity_=new_size*2;
    else
        capacity_=new_size+1;
    size_=new_size;
    auto * new_str=new char[capacity_]();
    std::memcpy(new_str, str_, size_ );
    delete [] str_;
    str_ = new_str;
}
void MyString::append(size_t count,char c)
{
    auto * c_as_str=new char[count]();
    std::fill_n(c_as_str,count,c);
    append(c_as_str,0,1);
    delete [] c_as_str;
}
void MyString::append(const char*c_str,size_t index,size_t count)
{
    if(default_append==count)
        count=std::strlen(c_str);
    const auto new_size=size_+count;
    if(capacity_+1<new_size)
        resize(new_size); 
    std::strncat(str_,c_str+index,count);
    size_=new_size;
}
void MyString::append(const std::string& str,size_t index,size_t count)
{
    append(str.c_str(),index,count);
}
void MyString::replace(size_t index, size_t count, const std::string &str)
{
    replace(index,count,str.c_str());
}
void MyString::replace(size_t index, size_t count, const char* str)
{
    if(str==nullptr)
        throw std::invalid_argument("nullptr passed to replace");
    const size_t arg_size=std::strlen(str);
    const size_t new_size{size_ - count+arg_size};
    const size_t old_rhs_start{index+count};
    const size_t new_rhs_start{old_rhs_start+arg_size-count};
    resize(std::max(new_size,size_));
    std::memmove(str_ + new_rhs_start, str_ + old_rhs_start, size_-old_rhs_start);
    std::memcpy(str_+index,str,arg_size);
    resize(new_size);
}
void MyString::erase(size_t index,size_t count)
{
    replace(index,count,"");
}
void MyString::shrink_to_fit()
{
    const auto real_size=std::strlen(str_);
    if(capacity_-1 > real_size)
        resize(real_size);
}
void MyString::insert(size_t index,const char * str,size_t size)
{
    if(size==default_append)
        size=std::strlen(str);
    char * temp=new char[size]();
    std::memcpy(temp,str,size);
    replace(index,0,temp);
    delete [] temp;
}
void MyString::insert(size_t index ,size_t count,char c)
{
    auto * temp=new char[count]();
    std::fill_n(temp,count,c);
    replace(index,0,temp);
    delete [] temp;
}
void MyString::insert(size_t index,const std::string&str,size_t size)
{
    replace(index,size,str.c_str());
}
int MyString::find(const char*str,size_t index)
{
    const auto result=std::strstr(str_+index,str);
    if(result!=nullptr)
        return static_cast<int>(result-str_);
    return -1;
}
int MyString::find(const std::string&str,size_t index)
{
    return find(str.c_str(),index);
}
MyString MyString::substr(size_t index,size_t count)
{
    if(default_append==count)
        count=size_-index;
    auto * temp=new char[count+1]();
    strncpy(temp,str_+index,count);
    MyString return_value(temp);
    delete [] temp;
    return return_value;
}
MyString MyString::operator+(const MyString&rhs)
{
    MyString copy(*this);
    copy.append(rhs.c_str());
    return copy;
}
MyString MyString::operator+(const char*rhs)
{
    MyString copy(*this);
    copy.append(rhs);
    return copy;
}
MyString MyString::operator+(const std::string&rhs)
{
    MyString copy(*this);
    copy.append(rhs.c_str());
    return copy;
}
MyString & MyString::operator+=(const char* rhs)
{
    append(rhs);
    return *this;
}
MyString & MyString::operator+=(const std::string&rhs)
{
    *this+=rhs.c_str();
    return *this;
}
MyString & MyString::operator+=(const MyString&rhs)
{
    *this+=rhs.c_str();
    return *this;
}
MyString & MyString::operator=(const char rhs )
{
    resize(1);
    str_[0]=rhs;
    return *this;
}
MyString & MyString::operator=(const char*rhs )
{
    MyString tmp=MyString(rhs);
    swap(*this,tmp);
    return *this;
}
MyString & MyString::operator=(const MyString& rhs)
{
    *this=rhs.c_str();
    return *this;
}
MyString & MyString::operator=(const std::string& rhs)
{
    *this=rhs.c_str();
    return *this;
}
MyString & MyString::operator=(MyString && rhs)
{
    if(this!=&rhs)
    {
        delete[] str_;
        str_=std::exchange(rhs.str_,nullptr);
        size_=std::exchange(rhs.size_,0);
        capacity_=std::exchange(rhs.capacity_,0);
    }
    return *this;
}
char MyString::operator[](size_t index) const
{
    return str_[index];
}
char & MyString::operator[](size_t index)
{
    return str_[index];
}
bool operator==(const MyString& lhs,const MyString &rhs)
{
    return std::strcmp(lhs.c_str(),rhs.c_str())==0;
}
bool operator!=(const MyString& lhs,const MyString &rhs)
{
    return !(lhs==rhs);
}
bool operator<(MyString const&lhs, MyString const &rhs)
{
    return strcmp(lhs.c_str(),rhs.c_str())<0;
}
bool operator>(MyString const&lhs, MyString const &rhs)
{
    return rhs<lhs;
}
bool operator>=(MyString const&lhs, MyString const &rhs)
{
    return !(lhs<rhs);
}
bool operator<=(MyString const&lhs, MyString const &rhs)
{
    return !(lhs>rhs);
}
void swap(MyString& first, MyString& second) // nothrow
{
        using std::swap;
        swap(first.capacity_, second.capacity_);
        swap(first.str_, second.str_);
        swap(first.size_,second.size_);
}
char & MyString::at(int index)
{
    if(index>=size_||index<0)
        throw std::out_of_range("invalid arg passed to MyString::at");
    return str_[index];
}
using it=MyString::iterator;
using cit=MyString::const_iterator;
using rit=MyString::reverse_iterator;
using rcit=MyString::const_reverse_iterator;
it MyString::begin()
{
    return it(&str_[0]);
}
it MyString::end()
{
    return it(&str_[size_]);
}
cit MyString::begin() const 
{
    return cit(&str_[0]);
}
cit MyString::end() const
{
    return cit(&str_[size_]);
}
cit MyString::cbegin() const
{
    return  MyString::end();
}
cit MyString::cend() const
{
    return  MyString::end();
}
rit MyString::rbegin()
{
    return reverse_iterator(&str_[size_-1]);
}
rit MyString::rend()
{
    return reverse_iterator(&str_[0]-1);
}
rcit MyString::rcbegin() const
{
    return rcit(&str_[size_-1]);
}
rcit MyString::rcend() const
{
    return rcit(&str_[0]-1);
}
rcit MyString::rbegin() const
{
    return rcit(&str_[size_-1]);
}
rcit MyString::rend() const
{
    return rcit(&str_[0]-1);
}
MyString::~MyString()
{
    delete [] str_;
}