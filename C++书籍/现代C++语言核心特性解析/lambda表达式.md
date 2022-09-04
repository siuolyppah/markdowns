# 过去STL如何实现谓词函数

如std::find_if、std::replace_if等。

编写纯虚函数或者仿函数。



# lambda表达式的语法（C++11）

```C++
[ captures ] ( params ) specifiers exception -> ret { body }
```



- captures：捕获列表。分为：
  - 按值捕获。
  - 按引用捕获。
- parms：可选的参数列表。
- specifiers：可选的限定符。如mutable。
- exception：可选的异常说明符。
- ret：返回值类型。
  - void可以不写。
  - 有返回值，也可以不写，由编译器推导。

> 由于很多都是可选的，最简单的lambda表达式型为`[]{}`



# 捕获列表

## 作用域

- 捕获列表中的变量，存在于两个作用域：
  - 目的是为了捕获变量：lambda表达式定义的函数作用域。
  - 目的是为了使用变量：lambda的函数体的作用域。

- ==被捕获的变量，必须是一个自动存储类型（即非静态的局部变量）==。

  ```C++
  int x = 0;
  
  int main()
  {
      int y = 0;
      static int z = 0;
      auto foo = [x, y, z] {};
  }
  ```

  以上代码将无法通过编译。

  1. 变量x和z不是自动存储类型的变量

  2. x不存在于lambda表达式定义的作用域。

     > 事实上，可以不捕获x，直接在函数体中使用x。



## 捕获值与捕获引用

- 捕获值：

  - 将变量x值的，复制到lambda表达式对象的内部。

  - 默认无法修改，是被捕获的值是const的。类似于类的常量成员函数。

    > 可通过使用`mutable`关键字修饰，从而对变量进行修改。

- 捕获引用：

  语法：`[&x]`，例如：

  ```C++
  int main()
  {
      int x = 5, y = 8;
      auto foo = [&x, &y] { return x * y; };
  }



## 特殊的捕获方法

lambda表达式的捕获列表除了指定捕获变量之外还有3种特殊的捕获方法：

- [this] —— 捕获this指针，捕获this指针可以让我们使用this类型的成员变量和函数。
- [=] —— 捕获lambda表达式定义作用域的全部变量的值，包括this。
- [&] —— 捕获lambda表达式定义作用域的全部变量的引用，包括this。



# lambda的实现原理

一个lambda表达式，就是一个函数对象（仿函数）。

```C++
#include 

class Bar
{
public:
    Bar(int x, int y) : x_(x), y_(y) {}
    int operator () ()
    {
        return x_ * y_;
    }
private:
int x_;
int y_;
};

int main()
{
    int x = 5, y = 8;
    auto foo = [x, y] { return x * y; };
    Bar bar(x, y);
    std::cout << "foo() = " << foo() << std::endl;
    std::cout << "bar() = " << bar() << std::endl;
}
```



lambda表达式在编译期会由编译器自动生成一个闭包类，在运行时由这个闭包类产生一个对象，我们称它为闭包。==在C++中，所谓的闭包可以简单地理解为一个匿名且可以包含定义时作用域上下文的函数对象==。



# 无状态lambda表达式

==对于无状态的lambda表达式，它可以隐式转换为函数指针==。

```C++
void f(void(*)(int)) {
}

int main()
{
    f([](int) {});
}
```

```C++
void f(void(&)(int)) {
}

int main()
{
    f(*[](int) {});
}
```





# 在STL中使用lambda

```C++
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	vector<int> v = { 1,2,3,4,5 };

	auto iter =std::find_if(v.cbegin(), v.cend(), [](int i) {
		return (i % 3) == 0;
		});

	cout << *iter << endl;
}
```





# 广义捕获（C++14）

广义捕获，实际上包含两种方式：

1. 简单捕获，C++11中的捕获方式，即[identifier]、[&identifier]以及[this]；

2. 初始化捕获；

   ```C++
   int main()
   {
       int x = 5;
       auto foo = [r = x + 1]{ return r; };
   }
   ```

   在闭包内部，r的值被初始化为6，而x不受影响。





初始化捕获，有两种典型用法：

1. 在捕获时，使用std::move对捕获列表变量x进行初始化：

   ```C++
   #include <string>
   
   int main()
   {
       std::string x = "hello c++ ";
       auto foo = [x = std::move(x)]{ return x + "world"; };
   }

2. 在异步调用时复制this对象，防止lambda表达式被调用时因原始this对象被析构造成未定义的行为：

   ```C++
   class Work
   {
     private:
       int value;
   
     public:
       Work() : value(42) {}
       std::future<int> spawn()
       {
           return std::async([=, tmp = *this]() -> int { return tmp.value; });
       }
   };
   
   
   int main()
   {
       std::future<int> f = foo();
       f.wait();
     std::cout << "f.get() = " << f.get() << std::endl;
   }



# 泛型lambda（C++14）

C++14标准让lambda表达式具备了模版函数的能力，我们称它为泛型lambda表达式。虽然具备模版函数的能力，但是它的定义方式却用不到template关键字。实际上==泛型lambda表达式语法要简单很多，我们只需要使用auto占位符即可==。

```C++
int main()
{
    auto foo = [](auto a) { return a; };
    int three = foo(3);
    char const* hello = foo("hello");
}
```

==泛型lambda表达式更多地利用了auto占位符的特性==，而lambda表达式本身并没有什么变化。



# 常量lambda表达式和捕获*this（C++17）

- 常量lambda表达式的主要特性体现在constexpr关键字上，请阅读constexpr的有关章节。

- 对于捕获this的增强：

  在捕获列表中直接添加[*this]，而无需初始化一个lambda形参。

  ```C++
  class Work
  {
    private:
      int value;
  
    public:
      Work() : value(42) {}
      std::future<int> spawn()
      {
          return std::async([=, *this]() -> int { return value; });
      }
  };
  ```

  在lambda表达式内也没有再使用tmp.value而是直接返回了value。

  [\*this]的语法让程序==生成了一个\*this对象的副本==并存储在lambda表达式内，可以在lambda表达式内直接访问这个复制对象的成员，消除了之前lambda表达式需要通过tmp访问对象成员的尴尬。



# 捕获[=,this]（C++20）

在C++17中，`=`会将this指针捕获在内。

而在C++20中，`=`不再捕获this指针，需要程序员手动捕获this指针。



例如：

```C++
[=, this]{}; // C++17 编译报错或者报警告， C++20成功编译
```





# 模板语法的泛型lambda表达式

```C++
auto f = []<typename T>(T const& x) {
    T copy = x;
    using Iterator = typename T::iterator;
};
```



# 可构造和可赋值的无状态lambda表达式

- 在C++20标准之前无状态的lambda表达式类型既不能构造也无法赋值，这阻碍了许多应用的实现。
- 在C++20，五状态的lambda表达式，支持了构造和赋值（不再是被删除的）。