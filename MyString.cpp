#include "MyString.hpp"
#include <cstring>
#include <stdexcept>
//Default c-tor
MyString::MyString():
    str_(new char[capacity_]())
{}
//C-style string c-tor
MyString::MyString(const char * c_str):
    size_(c_str ? std::strlen(c_str) : 0),
    capacity_(size_+1),
    str_( c_str ? new char[capacity_]() : nullptr)
{
    //same behaviour as std::string
    if(nullptr==c_str)
        throw std::invalid_argument("nullptr passed to C-tor");
    std::memcpy(str_,c_str,size_);
}
//std::string c-tor
MyString::MyString(const std::string& arg):
    MyString(arg.c_str())
{}
//std::initializer_list c-tor
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
    str_( c_str ? new char[capacity_]() : nullptr)
{
    if(nullptr==c_str)
        throw std::invalid_argument("nullptr passed to C-tor");
    const auto len=std::strlen(c_str);
    for(size_t i=0;i<count;++i)
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
    size_{copy.size_},
    capacity_{copy.capacity_},
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
//might cause some problems if new_size is very big
void MyString::resize(size_t new_size)
{
    //when changing size down, always allocate just enougn for the string
    if(capacity_>new_size+1)
        capacity_=new_size+1;
    else
    {
        //otherwise multiple capacity by the smallest power of 2, such that capacity_*(2^n)>new_size
        while(capacity_<new_size)
            capacity_*=2;
    }
    //reallocate and copy
    size_=new_size;
    auto * new_str=new char[capacity_]();
    std::memcpy(new_str, str_, size_ );
    delete [] str_;
    str_ = new_str;
}
void MyString::append(size_t count,char c)
{
    //make an array of count symbols and append it
    auto * c_as_str=new char[count]();
    std::memset(c_as_str,c,count);
    append(c_as_str,0,1);
    delete [] c_as_str;
}
void MyString::append(const char*c_str,size_t index,size_t count)
{
    //using default parameter for count =2^64-1
    if(default_append==count)
        count=std::strlen(c_str);
    //reallocate if needed and copy to the end
    const auto new_size=size_+count;
    if(capacity_+1<new_size)
        resize(new_size); 
    //std::strncat(str_,c_str+index,count);
    std::memcpy(str_+size_,c_str,count);
    size_=new_size;
}
void MyString::append(const std::string& str,size_t index,size_t count)
{
    //awoiding code duplication 
    append(str.c_str(),index,count);
}
//TODO check with tests
void MyString::replace(size_t index, size_t count, const char* str)
{
    if(str==nullptr)
        throw std::invalid_argument("nullptr passed to replace");
    /*
    calculate indexes of left and right parts of the string after replacement,
    then move the right part with std::memmove and insert the replacer in the
    formed space between left and right parts, as well as shrinking the capacity
    to new_size
    */
    const size_t arg_size{std::strlen(str)};
    const size_t new_size{size_ - count+arg_size};
    const size_t old_rhs_start{index+count};
    const size_t new_rhs_start{old_rhs_start+arg_size-count};
    //resize(std::max(new_size,size_));
    resize(new_size);
    std::memmove(str_ + new_rhs_start, str_ + old_rhs_start, size_-old_rhs_start);
    std::memcpy(str_+index,str,arg_size);
    //resize(new_size);
}
void MyString::replace(size_t index, size_t count, const std::string &str)
{
    replace(index,count,str.c_str());
}
//erasing n chars at index i is the same as replacing n chars at index i with an empty string
void MyString::erase(size_t index,size_t count)
{
    replace(index,count,"");
}
//reduce capacity to minimum of size+1 if capacity is bigger than the minimum
void MyString::shrink_to_fit()
{
    const auto real_size=std::strlen(str_);
    if(capacity_-1 > real_size)
        resize(real_size);
}
//inserting size chars at index position is the same as replacing 0 chars at index position with the replacer
void MyString::insert(size_t index,const char * str,size_t size)
{
    if(nullptr ==str)
        throw std::invalid_argument("nullptr passed to MyString::insert");
    if(size==default_append)
        size=std::strlen(str);        
    char * temp=new char[size]();
    std::memcpy(temp,str,size);
    replace(index,0,temp);
    delete [] temp;
}
//same as above but fill a temp string
void MyString::insert(size_t index ,size_t count,char c)
{
    auto * temp=new char[count]();
    std::memset(temp,c,count);
    replace(index,0,temp);
    delete [] temp;
}
void MyString::insert(size_t index,const std::string&str,size_t size)
{
    replace(index,0,str.c_str());
}
/*
find using builtin strstr,
and since it returns a pointer, cast ptr difference to int an return
*/
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
/*
copy count characters to a string,then construct a temp MyString based on the string,
and hope that compiler uses RVO here :)
*/
//TODO add out of bounds exception
MyString MyString::substr(size_t index,size_t count)
{
    if(default_append==count)
        count=size_-index;
    auto * temp=new char[count+1]();
    std::memcpy(temp,str_+index,count);
    MyString return_value(temp);
    delete [] temp;
    return return_value;
}
//remake as friend i guess
MyString MyString::operator+(const MyString&rhs)
{
    //MyString copy(*this);
    //copy.append(rhs.c_str());
    return *this+rhs.c_str();
}
MyString MyString::operator+(const char*rhs)
{
    MyString copy(*this);
    copy.append(rhs);
    return copy;
}
MyString MyString::operator+(const std::string&rhs)
{
    //MyString copy(*this);
    //copy.append(rhs.c_str());
    return *this+rhs.c_str();
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
//using copy-and swap idiom, 
MyString & MyString::operator=(const char*rhs )
{
    MyString tmp{rhs};
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
//move assignment, checking for self-assigning
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
void swap(MyString& first, MyString& second)
{
    std::swap(first.capacity_, second.capacity_);
    std::swap(first.str_, second.str_);
    std::swap(first.size_,second.size_);
}
//throw if out of range(obvious)
char & MyString::at(int index)
{
    if(static_cast<size_t>(index)>=size_||index<0)
        throw std::out_of_range("invalid arg passed to MyString::at");
    return str_[index];
}
//some aliasing
using it=MyString::iterator;
using cit=MyString::const_iterator;
using rit=MyString::reverse_iterator;
using rcit=MyString::const_reverse_iterator;
//all iterators methods called from class instance, where c-version differs only in 'const' modifier
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