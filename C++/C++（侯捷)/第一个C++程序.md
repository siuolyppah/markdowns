# class without pointer member

```C++
#ifndef __COMPLEX__
#define __COMPLEX__

class complex;
complex& __doapl(complex* ths, const complex& r);
std::ostream& operator<< (std::ostream& os, complex& comp);

class complex {
public:
	complex(double r = 0, double i = 0) :re(r), im(i) {}
	complex& operator +=(const complex& r);

private:
	double re, im;

	friend complex& __doapl(complex*, const complex&);
	friend std::ostream& operator<< (std::ostream& os, complex& comp);
};



inline std::ostream&
operator<<(std::ostream& os, complex& c) {
	os << '(' << c.im << ',' << c.re << ')';

	return os;
}

inline complex& __doapl(complex* ths, const complex& r) {
	ths->re += r.re;
	ths->im += r.im;

	return *ths;
}

inline complex&
complex::operator+=(const complex& r) {
	return __doapl(this, r);
}

#endif // !__COMPLEX__
```





要点回顾：

- 构造方法的初始化列表
- 优先考虑添加const
- 尽量以引用传递及返回

# class with pointer member

```C++
#ifndef __MYSTRING__
#define __MYSTRING__

class String
{
public:                                 
   String(const char* cstr=0);                     
   String(const String& str);                    
   String& operator=(const String& str);         
   ~String();                                    
   char* get_c_str() const { return m_data; }
private:
   char* m_data;
};

#include <cstring>

inline
String::String(const char* cstr)
{
   if (cstr) {
      m_data = new char[strlen(cstr)+1];
      strcpy(m_data, cstr);
   }
   else {   
      m_data = new char[1];
      *m_data = '\0';
   }
}

inline
String::~String()
{
   delete[] m_data;
}

inline
String& String::operator=(const String& str)
{
   if (this == &str)
      return *this;

   delete[] m_data;
   m_data = new char[ strlen(str.m_data) + 1 ];
   strcpy(m_data, str.m_data);
   return *this;
}

inline
String::String(const String& str)
{
   m_data = new char[ strlen(str.m_data) + 1 ];
   strcpy(m_data, str.m_data);
}

#include <iostream>
using namespace std;

ostream& operator<<(ostream& os, const String& str)
{
   os << str.get_c_str();
   return os;
}

#endif
```


