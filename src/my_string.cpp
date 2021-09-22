#include "../include/my_string.hpp"
#include "../include/aho_corasick.hpp"
//Default c-tor
MyString::MyString():
    str_{new char[capacity_]()}
{}
//std::string c-tor
MyString::MyString(const std::string& arg):
    MyString{arg.c_str()}
{}
//std::initializer_list c-tor
MyString::MyString(std::initializer_list<char> init_list):
    size_{init_list.size()},
    capacity_{size_+1},
    str_{new char[capacity_]()}
{
    size_t i=0;
    for(const auto elem:init_list)
        str_[i++]=elem;
}
//count string constructor
MyString::MyString(const char * c_str,size_t count):
    size_{c_str ? std::strlen(c_str)*count : 0},
    capacity_{size_+1},
    str_{ c_str ? new char[capacity_]() : nullptr}
{
    if(nullptr==c_str)
        throw std::invalid_argument("nullptr passed to constructor");
    const auto len{std::strlen(c_str)};
    for(size_t i=0;i<count;++i)
        std::memcpy(str_+len*i,c_str,len);
}
//count char constructor
MyString::MyString(char c,size_t count):
    size_{count},
    capacity_{size_+1},
    str_{new char[capacity_]()}
{
    std::memset(str_,c,size_);
}
//Copy constructor
MyString::MyString(const MyString&copy):
    MyString{copy.c_str()}
{
}
//Move constructor 
MyString::MyString(MyString&&rhs):
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
        while(capacity_<new_size+1)
            capacity_*=2;
    }
    //reallocate and copy
    auto * new_str{new char[capacity_]()};
    if(new_size<size_)
        size_=new_size;
    std::memcpy(new_str, str_, size_ );
    delete [] str_;
    size_=new_size;
    str_ = new_str;
}
void MyString::append(size_t count,char c)
{
    //make an array of count symbols and append it
    std::unique_ptr<char[]>c_as_str{new char[count+1]()};
    std::memset(c_as_str.get(),c,count);
    append(c_as_str.get(),0,count);
}
void MyString::append(const char*c_str,size_t index,size_t count)
{
    //using default parameter for count =2^64-1
    if(default_append==count)
        count=std::strlen(c_str);
    //reallocate if needed and copy to the end
    const auto new_size{size_+count};
    const auto old_size{size_};
    if(capacity_<new_size+1)
        resize(new_size); 
    std::memcpy(str_+old_size,c_str+index,count);
    size_=new_size;
}
void MyString::append(const std::string& str,size_t index,size_t count)
{
    //awoiding code duplication 
    append(str.c_str(),index,count);
}
void MyString::replace(size_t index, size_t count, const char* str)
{
    if(str==nullptr)
        throw std::invalid_argument("nullptr passed to replace");
    /*
    calculate indexes of left and right parts of the string after replacement,
    then move the right part with std::memmove and insert the replacer in the
    formed space between left and right parts
    */
    const size_t arg_size{std::strlen(str)};
    const size_t new_size{size_-count+arg_size};
    const size_t old_rhs_start{index+count};
    const size_t new_rhs_start{old_rhs_start+arg_size-count};
    const size_t old_size{size_};
    //if replacing makes string bigger we resize it so we have space for moving part to the right from replaced substr
    if(new_size>size_)
        resize(new_size);
    std::memmove(str_ + new_rhs_start, str_ + old_rhs_start, old_size-old_rhs_start);
    std::memcpy(str_+index,str,arg_size);
    //if we didnt increase the string capacity, shrink capacity now since string did decrease in size
    if(new_size<old_size)
        resize(new_size);
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
    const auto real_size{std::strlen(str_)};
    if(capacity_-1 > real_size)
        resize(real_size);
}
//inserting size chars at index position is the same as replacing 0 chars at index position with the replacer
void MyString::insert(size_t index,const char * str,size_t size)
{
    if(nullptr ==str)
        throw std::invalid_argument("nullptr passed to MyString::insert");
    if(index>size)
        throw std::out_of_range("MyString::insert : index was out of range");
    if(size==default_append)
        size=std::strlen(str);        
    std::unique_ptr<char[]>temp{new char[size+1]()};
    std::memcpy(temp.get(),str,size);
    replace(index,0,temp.get());
}
//same as above but fill a temp string
void MyString::insert(size_t index ,size_t count,char c)
{
    std::unique_ptr<char[]> temp{new char[count+1]()};
    std::memset(temp.get(),c,count);
    replace(index,0,temp.get());
}
void MyString::insert(size_t index,const std::string&str,size_t size)
{
    insert(index,str.c_str(),size);
}
/*
find using builtin strstr,
and since it returns a pointer, cast ptr difference to int an return
*/
int MyString::find(const char*str,size_t index)
{
    if(index>size_)
        throw std::out_of_range("MyString::find index was out of range!");
    if(nullptr==str)
        throw std::invalid_argument("nullptr passed to MyString::find");
    const auto result{std::strstr(str_+index,str)};
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
MyString MyString::substr(size_t index,size_t count)
{
    if(default_append==count)
        count=size_-index;
    //maybe i should throw an exception
    if(count+index>size_)
        throw std::out_of_range("MyString::substr count + index was bigger than string is ");
    std::unique_ptr<char[]> temp{new char[count+1]()};
    std::memcpy(temp.get(),str_+index,count);
    //idk if lifetime of temp ends before or after return 
    MyString return_value{temp.get()};
    return return_value;
}
MyString operator+(const MyString&lhs,const MyString &rhs)
{
    MyString copy{lhs};
    return (copy+rhs.str_);
}
MyString operator+(const MyString&lhs,const char*rhs)
{
    MyString copy{lhs};
    copy.append(rhs);
    return copy;
}
MyString operator+(const MyString&lhs,const std::string&rhs)
{
    MyString copy{lhs};
    copy.append(rhs.c_str());
    return copy;
}
MyString operator+(const char*lhs,const MyString&rhs)
{
    MyString copy{lhs};
    copy.append(rhs.c_str());
    return copy;
}
MyString operator+(const std::string&lhs,const MyString&rhs)
{
    MyString copy{lhs};
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
        throw std::out_of_range("MyString::at : index out of range");
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
    return it{&str_[0]};
}
it MyString::end()
{
    return it{&str_[size_]};
}
cit MyString::begin() const 
{
    return cit{&str_[0]};
}
cit MyString::end() const
{
    return cit{&str_[size_]};
}
cit MyString::cbegin() const
{
    return MyString::begin();
}
cit MyString::cend() const
{
    return MyString::end();
}
rit MyString::rbegin()
{
    return rit{&str_[size_-1]};
}
rit MyString::rend()
{
    return rit{&str_[0]-1};
}
rcit MyString::rcbegin() const
{
    return rcit{&str_[size_-1]};
}
rcit MyString::rcend() const
{
    return rcit{&str_[0]-1};
}
rcit MyString::rbegin() const
{
    return rcit{&str_[size_-1]};
}
rcit MyString::rend() const
{
    return rcit{&str_[0]-1};
}
MyString::~MyString()
{
    delete [] str_;
}