import swig.my_string_swig as d
import mystring as ms
a=ms.MyString("abc")
b=ms.MyString("abc",3)
i=1234
c=ms.MyString(i)
d=ms.MyString(0.0045)
print(a)
print(b)
print(c)
print(d)
c=ms.MyString("d")
print(c)
b.clear()
print(b)
b.insert(0,"aboba")
b.insert(3,"bebra",5)
b.append("a")
b.append("abc",1)
b.append("zxc",1,2)
b+=ms.MyString("zzz")
print(b)