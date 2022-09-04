> ==着重理解“说明符”。==

# 回顾typeof和typeid

在C++11标准发布以前，GCC的扩展提供了一个名为typeof的运算符。通过该运算符可以获取操作数的具体类型。这让使用GCC的程序员在很早之前就具有了对对象类型进行推导的能力，例如：

```C++
int a = 0;
typeof(a) b = 5;
```

>**typeof并非C++标准**



C++标准还提供了一个`typeid运算符`来获取与目标操作数类型有关的信息。获取的类型信息会包含在一个类型为std::type_info的对象里。我们可以调用成员函数name获取其类型名，例如：

```C++
int x1 = 0;
double x2 = 5.5;
std::cout << typeid(x1).name() << std::endl;
std::cout << typeid(x1 + x2).name() << std::endl;
std::cout << typeid(int).name() << std::endl;
```

需要注意：

1. name返回的类型名没有明确规范，因编译器而异。

2. typeid的返回值是一个左值，且其生命周期一直被扩展到程序生命周期结束。

3. typeid返回的std::type_info删除了复制构造函数，若想保存std::type_info，只能获取其引用或者指针，例如：

   ```C++
   auto t1 = typeid(int);     // 编译失败，没有复制构造函数无法编译
   auto &t2 = typeid(int);    // 编译成功，t2推导为const std::type_info&
   auto t3 = &typeid(int);    // 编译成功，t3推导为const std::type_info*

4. typeid的返回值总是忽略类型的 cv 限定符，也就是typeid(const T)==typeid(T))。



>虽然typeid可以获取类型信息并帮助我们判断类型之间的关系，但遗憾的是，它并==不能像typeof那样在编译期就确定对象类型==。



# 使用decltype说明符(C++11)

C++11标准引入了decltype说明符，使==用decltype说明符可以获取**对象**或者**表达式**的类型==，其语法与typeof类似：

```C++
int x1 = 0;
decltype(x1) x2 = 0;
std::cout << typeid(x2).name() << std::endl;  // x2的类型为int

double x3 = 0;
decltype(x1 + x3) x4 = x1 + x3;
std::cout << typeid(x4).name() << std::endl;  // x1+x3的类型为double

decltype({1, 2}) x5;                          // 编译失败，{1, 2}不是表达式
```



decltype用于 函数返回类型后置（**C++11**）：

```C++
template<class T1, class T2>
auto sum(T1 a1, T2 a2)->decltype(a1 + a2)
{
  return a1 + a2;
}

auto x4 = sum(5, 10.5);
```



上述用法只推荐在C++11标准的编译环境中使用，==因为C++14标准已经支持对auto声明的返回类型进行推导==了，所以以上代码可以简化为：

```C++
template<class T1, class T2>
auto sum(T1 a1, T2 a2)
{
  return a1 + a2;
}

auto x5 = sum(5, 10.5);
```

> 但在某些场景下，auto作为返回类型的占位符不能满足需求。如：
>
> ```C++
> template<class T>
> auto return_ref(T& t)
> {
>   return t;
> }
> 
> int x1 = 0;
> static_assert(
>     std::is_reference_v<decltype(return_ref(x1))>// 编译错误，返回值不为引用类型
>     );
> ```
>
> 期望return_ref返回的是一个T的引用类型，但是如果编译此段代码，则必然会编译失败，因为auto被推导为值类型，这就是第3章所讲的auto推导规则2。如果想正确地返回引用类型，则需要用到decltype说明符。



# decltype的推导规则

decltype(e)（其中e的类型为T）的推导规则有5条：

1. 如果e是一个未加括号的标识符表达式（结构化绑定除外）或者未加括号的类成员访问，则decltype(e)推断出的类型是e的类型T。如果并不存在这样的类型，或者e是一组重载函数，则无法进行推导。

   >[表达式 (C++) | Microsoft Docs](https://docs.microsoft.com/zh-cn/cpp/cpp/expressions-cpp?view=msvc-170)

2. 如果e是一个函数调用或者仿函数调用，那么decltype(e)推断出的类型是其返回值的类型。

3. 如果e是一个类型为T的左值，则decltype(e)是T&。

4. 如果e是一个类型为T的将亡值，则decltype(e)是T&&。

5. 除去以上情况，则decltype(e)是T。

> 总结：
>
> - 未加括号的：返回T。
>   - 标识符表达式（除结构或绑定）
>   - 类成员访问
>
> - 函数调用：推导出函数返回值的类型。
> - T类型左值：推导出T&。
> - T类型右值：推导出T&&。
> - 



```C++
const int&& foo();
int i;
struct A {
    double x;
};
const A* a = new A();

decltype(foo());         // decltype(foo())推导类型为const int&&
decltype(i);             // decltype(i)推导类型为int
decltype(a->x);          // decltype(a->x)推导类型为double
decltype((a->x));        // decltype((a->x))推导类型为const double&
```

在上面的代码中，decltype(foo())满足规则2和规则4，foo函数的返回类型是const int&&，所以推导结果也为const int&&；decltype(i)和decltype(a->x)很简单，满足规则1，所以其类型为int和double；最后一句代码，由于decltype((a->x))推导的是一个带括号的表达式(a->x)，因此规则1不再适用，但很明显a->x是一个左值，又因为a带有const限定符，所以其类型被推导为const double&。



# cv限定符的推导

通常情况下，decltype(e)所推导的类型会同步e的cv限定符，比如：

```C++
const int i = 0;
decltype(i);    // decltype(i)推导类型为const int
```



但是当e是未加括号的成员变量时，父对象表达式的cv限定符会被忽略，不能同步到推导结果：

```C++
struct A {
    double x;
};
const A* a = new A();
decltype(a->x);    // decltype(a->x)推导类型为double, const属性被忽略
```

在上面的代码中，a被声明为const类型，如果想在代码中改变a中x的值，则肯定会编译失败。但是decltype(a->x)却得到了一个没有const属性的double类型。

当然，如果我们给a->x加上括号，则情况会有所不同：

```C++
struct A {
    double x;
};
const A* a = new A();
decltype((a->x));    // decltype((a->x))推导类型为const double&
```

总的来说，当e是加括号的数据成员时，父对象表达式的cv限定符会同步到推断结果。



# decltype(auto)（C++14）

- 用于告诉编译器用decltype的推导表达式规则==来推导auto==。

- decltype(auto)必须单独声明，即它不能结合指针、引用以及cv限定符。

```C++
int i;
int&& f();
auto x1a = i;                   // x1a推导类型为int
decltype(auto) x1d = i;         // x1d推导类型为int
auto x2a = (i);                 // x2a推导类型为int
decltype(auto) x2d = (i);       // x2d推导类型为int&
auto x3a = f();                 // x3a推导类型为int
decltype(auto) x3d = f();       // x3d推导类型为int&&
auto x4a = { 1, 2 };            // x4a推导类型为std::initializer_list<int>
decltype(auto) x4d = { 1, 2 };  // 编译失败, {1, 2}不是表达式
auto *x5a = &i;                 // x5a推导类型为int*
decltype(auto)*x5d = &i;        // 编译失败，decltype(auto)必须单独声明
```

