#ifndef MY_STRING_H
#define MY_STRING_H
#include <map>
#include <string>
#include <iostream>
#include <type_traits>
#include <cstring>
#include <inttypes.h>
#include <stdexcept>
#include <memory>
#include <vector>
#include "aho_corasick.hpp"
class MyString //class should be final, but it messes up wrappers
{
    static constexpr size_t default_append{static_cast<size_t>(-1)};
    size_t size_{0};
    size_t capacity_{1};
    char *str_{nullptr};
    void resize(size_t);
    template <typename ArgType>
    ArgType to_impl(std::true_type const &) const
    {
        static constexpr auto is_signed{std::is_signed_v<ArgType>};
        using ret_val_type=std::conditional_t<is_signed, int64_t, uint64_t>;
        static constexpr auto format{is_signed?"%" PRId64 "":"%" PRIu64 ""};
        ret_val_type ret_val{0};
        auto scanf_result{sscanf(str_, format, &ret_val)};
        if(scanf_result<1)
            throw InvalidConversion("Could not convert to an integer");
        return static_cast<ArgType>(ret_val);
    }
    template <typename ArgType>
    ArgType to_impl(std::false_type const &) const
    {
        auto ret_val{0.0};
        if(sscanf(str_, "%lf", &ret_val)<1)
            throw InvalidConversion("Could not convert to a float");
        return static_cast<ArgType>(ret_val);
    }
public:
    using AhoCorasickResult=std::map < std::string, std::vector <int> >;
    //making iterator a template, so we dont have to redeclare similar code 4 times for different iterators
    template <bool IsConst,bool IsReverse>
    struct Iterator
    {
        //defining iterator traits for our iterator using tag dispatch(?)
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = char;
        using pointer = std::conditional_t<IsConst, value_type const *, value_type *>;
        using reference = std::conditional_t<IsConst, value_type const &, value_type &>;
        //c-tor
        Iterator(pointer ptr) : ptr_{ptr} { }
        //using SFINAE to "enable"(remove from overload resolution) non const or const * operator based on constness of iterator
        template< bool _Const = IsConst >
        std::enable_if_t< _Const, reference >
        operator*()
        {
            return *ptr_;
        }
        template< bool _Const = IsConst >
        std::enable_if_t< !_Const, reference >
        operator*()
        {
            return *ptr_; 
        }
        pointer operator->()
        {
            return ptr_;
        }
        /*
        same behaviour as stl container iterators,
        since SomeClassInstance.rend() actually points to the [-1] element of the underlying sequence,
        it cant be used for inserting something into the container, therefore we provide any reverse iterator
        with a base() method, which return and instance of a non-reverse version, which satisfies the following 
        condition &*(ReverseIterator.base() - 1) == &*ReverseIterator, thus simply pointing to the next element
        */
        template< bool _IsReverse = IsReverse >
        std::enable_if_t< _IsReverse, Iterator<IsConst,!IsReverse> >
        base()
        {
            return Iterator<IsConst,!_IsReverse>(ptr_+1);
        }
        Iterator &operator++()
        {
            if constexpr(!IsReverse)
                ptr_++;
            else
                ptr_--;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator<IsConst,IsReverse> tmp = *this;
            if constexpr(!IsReverse)
                ++(*this);
            else
                --(*this);
            return tmp;
        }
        friend bool operator==(const Iterator &a, const Iterator &b)
        {
            return a.ptr_ == b.ptr_;
        }
        friend bool operator!=(const Iterator &a, const Iterator &b)
        {
            return a.ptr_ != b.ptr_;
        }
        private:
        /*
        since this is a forward iterator, we dont need decrement as a public method,
        but it can be used to define increment for reverse iterator
        */
        template< bool _IsReverse = IsReverse >
        std::enable_if_t< _IsReverse, reference >
        operator--()
        {
            ptr_--;
            return *ptr_; 
        }
        pointer ptr_=nullptr;
    };
    class InvalidConversion: public std::exception
    {
        public:
        explicit InvalidConversion(const char* string="Invalid conversion!"):msg_{new char[std::strlen(string)+1]()}
        {
            strcpy(msg_,string);
        }
        explicit InvalidConversion(const std::string & string):InvalidConversion{string.c_str()}{}
        const char* what () const noexcept
        {
            return msg_;
        }
        ~InvalidConversion() noexcept
        {
            delete [] msg_;
        }
        private:
        char * msg_=nullptr;
    };
    /*
    we can define methods using iterators just by calling 
    the non-iterator version with the parameter of it.operator->()-str_(index)
    we also want to ensure that callers use base() on reverse iterator call, therefore
    we disable these methods for reverse iterators using sfinae
    */
    //use base() for reverse iterators
    //less characters is better (i guess)
    template <bool IsConst>
    void
    insert(Iterator<IsConst,false> it, size_t count , char c)
    {
        insert(static_cast<size_t>(it.operator->()-str_),count,c);
    }
    template <bool IsConst>
    void
    insert(Iterator<IsConst,false> it, const char *str, size_t count= default_append)
    {
        insert(static_cast<size_t>(it.operator->()-str_),str,count);
    }
    template <bool IsConst>
    void    
    insert(Iterator<IsConst,false> it, const std::string & str, size_t count = default_append)
    {
        insert(static_cast<size_t>(it.operator->()-str_),str,count);
    }
    template <bool IsConst, bool IsReverse>
    void
    erase(Iterator<IsConst,false> it,size_t count )
    {
        erase(static_cast<size_t>(it.operator->()-str_),count);
    }
    template <bool IsConst, bool IsReverse>
    void
    append(const std::string&str,Iterator<IsConst,false>it, size_t count=default_append)
    {
        append(str,static_cast<size_t>(it.operator->()-str_),count);
    }
    template <bool IsConst, bool IsReverse>
    void
    append(const char *str,Iterator<IsConst,false>it, size_t count=default_append)
    {
        append(str,static_cast<size_t>(it.operator->()-str_),count);
    }
    template <bool IsConst>
    void
    replace(Iterator<IsConst,false>it,size_t count,const char*str)
    {
        replace(static_cast<size_t>(it.operator->()-str_),count,str);
    }
    template <bool IsConst>
    void replace(Iterator<IsConst,false>it ,size_t count,const std::string&str)
    {
        replace(static_cast<size_t>(it.operator->()-str_),count,str);
    }
    template <bool IsConst>
    MyString
    substr(Iterator<IsConst,false> it, size_t count=default_append) const
    {
        return substr(static_cast<size_t>(it.operator->()-str_),count);
    }
    template <bool IsConst>
    int
    find(const char*str, Iterator<IsConst,false> it) const
    {
        return find(str,static_cast<size_t>(it.operator->()-str_));
    }
    template <bool IsConst>
    int
    find(const std::string&str, Iterator<IsConst,false> it) const
    {
       return find(str,static_cast<size_t>(it.operator->()-str_));
    }
    MyString();
    MyString(const std::string &);
    MyString(std::initializer_list<char>);
    MyString(MyString &&);
    MyString(const char *,size_t =default_append);
    MyString(char, size_t);
    MyString(const MyString &);
    template<typename T,std::enable_if_t<std::is_integral<T>::value, bool> = true>
    MyString(T arg):size_{128},capacity_{size_+1},str_{new char[capacity_]()}
    {
        static constexpr auto is_signed{std::is_signed_v<T>};
        static constexpr auto format{is_signed?"%" PRId64 "":"%" PRIu64 ""};
        using arg_type=std::conditional_t<is_signed, int64_t, uint64_t>;
        std::snprintf(str_,size_,format,static_cast<arg_type>(arg));
    }
    template<typename T,std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
    MyString(T arg):size_{128},capacity_{size_+1},str_{new char[capacity_]()}
    {
        if constexpr(std::is_same<float,T>::value)
            std::snprintf(str_,size_,"%f",arg);
        else
            std::snprintf(str_,size_,"%lf",arg);
    }
    friend void swap(MyString &first, MyString &second);
    char *c_str() const;
    char *data() const;
    size_t size() const;
    size_t length() const;
    bool empty() const;
    size_t capacity() const;
    void clear();
    void append(size_t, char);
    void append(const char *c_str, size_t = 0, size_t = default_append);
    void append(const std::string &str, size_t = 0, size_t = default_append);
    void replace(size_t, size_t, const char *);
    void replace(size_t, size_t, const std::string &);
    void insert(size_t, size_t, char);
    void insert(size_t, const char *, size_t = default_append);
    void insert(size_t, const std::string &, size_t = default_append);
    MyString substr(size_t, size_t = default_append) const;
    int find(const char *, size_t = 0) const;
    int find(const std::string &, size_t = 0) const;
    AhoCorasickResult find(const std::vector<std::string>&) const;
    void erase(size_t, size_t);
    void shrink_to_fit();
    friend MyString operator+(const MyString &,const MyString &);
    friend MyString operator+(const char *,const MyString &);
    friend MyString operator+(const std::string &,const MyString &);
    friend MyString operator+(const MyString &,const char *);
    friend MyString operator+(const MyString &,const std::string &);
    MyString &operator+=(const MyString &);
    MyString &operator+=(const std::string &);
    MyString &operator+=(const char *);
    MyString &operator=(const char);
    MyString &operator=(const char *);
    MyString &operator=(const MyString &);
    MyString &operator=(const std::string &);
    MyString &operator=(MyString &&);
    char operator[](size_t) const;
    char &operator[](size_t);
    friend bool operator==(MyString const &, MyString const &);
    friend bool operator<(MyString const &, MyString const &);
    friend bool operator>(MyString const &, MyString const &);
    friend bool operator<=(MyString const &, MyString const &);
    friend bool operator>=(MyString const &, MyString const &);
    friend bool operator!=(MyString const &, MyString const &);
    char & at(int);
    template <typename T>
    friend std::basic_ostream<T> &operator<<(std::basic_ostream<T> &, const MyString &);
    template <typename T>
    friend std::basic_istream<T> &operator>>(std::basic_istream<T> &, MyString &);
    template <typename ArgType>
    ArgType to() const
    {
        return to_impl<ArgType>(std::is_integral<ArgType>{});
    }
    using const_iterator=Iterator<true,false>;
    using iterator=Iterator<false,false>;
    using const_reverse_iterator=Iterator<true,true>;
    using reverse_iterator=Iterator<false,true>;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rcbegin() const;
    const_reverse_iterator rcend() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    ~MyString();
};
template <typename T>
std::basic_ostream<T> &operator<<(std::basic_ostream<T> &out, const MyString &str)
{
    out << (str.str_);
    return out;
}
template <typename T>
std::basic_istream<T> &operator>>(std::basic_istream<T> &in, MyString &str)
{
    char ch;
    while (in.get(ch) && ch != '\n')
    {
        str.append(1, ch);
    }
    return in;
}
#endif