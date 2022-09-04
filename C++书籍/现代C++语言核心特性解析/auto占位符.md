> ==着重理解“占位符”。==

# 重新定义的auto关键字（推导类型）(C++11)

>在C++98中，使用auto来声明==自动变量==，即拥有自动生命期的变量。
>
>显然这是多余的，现在我们几乎不会使用它。



C++11标准赋予了auto新的含义：声明变量时根据初始化表达式自动==推断该**变量的类型**、声明函数时**函数返回值的占位符**==。

```C++
auto i = 5;                      // 推断为int
auto str = "hello auto";         // 推断为const char*
auto sum(int a1, int a2)->int    // 返回类型后置，auto为返回值占位符
{
    return a1+a2;
}
```



auto的4个注意点：

- 当用一个auto关键字声明多个变量的时候，==编译器遵从由左往右的推导规则，以最左边的表达式推断auto的具体类型==：

  ```C++
  int n = 5;
  auto *pn = &n, m = 10;
  ```

  因为&n类型为int *，所以pn的类型被推导为int *，auto被推导为int，于是m被声明为int类型，可以编译成功。

- 当使用条件表达式初始化auto声明的变量时，编译器==总是使用表达能力更强的类型==：

  ```C++
  auto i = true ? 5 : 8.0;    // i的数据类型为double
  ```

  虽然能够确定表达式返回的是int类型，但是i的类型依旧会被推导为表达能力更强的类型double。

- 虽然C++11标准已经支持在声明成员变量时初始化（见第8章），但是==auto却无法在这种情况下声明非静态成员变量==：

  ```C++
  struct sometype {
      auto i = 5;    // 错误，无法编译通过
  };
  ```

  在C++11中静态成员变量是可以用auto声明并且初始化的，不过前提是auto必须使用const限定符：

  ```C++
  struct sometype {
      static const auto i = 5;
  };
  ```

  幸运的是，在C++17标准中，对于静态成员变量，auto可以在没有const的情况下使用，例如：

  ```C++
  struct sometype {
      static inline auto i = 5;    // C++17
  };
  ```

  > 总结：
  >
  > - auto不能声明非静态成员变量。
  > - 对于静态成员变量：
  >   - 在C++11中，必须是const的。
  >   - 在C++17中，可以没有const修饰。

- 按照C++20之前的标准，无法==在函数形参列表中使用auto声明形参==（注意，在C++14中，auto可以为lambda表达式声明形参）：

  ```C++
  void echo(auto str) {…} // C++20之前编译失败，C++20编译成功
  ```

  另外，auto也可以和new关键字结合。当然，我们通常不会这么用，例如：

  ```C++
  auto i = new auto(5);
  auto* j = new auto(5);
  ```

  编译器实际上进行了两次推导，第一次是auto(5)，auto被推导为int类型，于是new int的类型为int *，再通过int *推导i和j的类型。



# 推导规则

>也可参见[auto (C++) - 维基百科，自由的百科全书 (wikipedia.org)](https://zh.wikipedia.org/wiki/Auto_(C%2B%2B))

1. 如果auto声明的变量是==按值初始化（声明变量时没有使用引用和指针）==，则推导出的类型==会忽略const和volatile限定符==。

   ```C++
   const int i = 5;
   auto j = i;        // auto推导类型为int，而非const int
   auto &m = i;       // auto推导类型为const int，m推导类型为const int&
   auto *k = i;       // auto推导类型为const int，k推导类型为const int*
   const auto n = j;  // auto推导类型为int，n的类型为const int
   ```

   在上面的代码中，虽然i是const int类型，但是因为按值初始化会忽略cv限定符，所以j的推导类型是int而不是const int。而m和k分别按引用和指针初始化，因此其cv属性保留了下来。另外，可以用const结合auto，让n的类型推导为const int。

   >==如果auto关键字还带上&号，声明引用类型，则不执行const剥除（const-stripping）==

2. 使用auto声明变量初始化时，目标对象如果是引用，则==引用属性会被忽略==：

   ```C++
   int i = 5;
   int &j = i;
   auto m = j;    // auto推导类型为int，而非int&
   ```

   虽然j是i的引用，类型为int&，但是在推导m的时候会忽略其引用。

3. ==使用auto和万能引用声明变量时（见第6章），对于左值会<u>**将auto**</u>推导为引用类型==：

   ```C++
   int i = 5;
   auto&& m = i;    // auto推导类型为int& （这里涉及引用折叠的概念）
   auto&& j = 5;    // auto推导类型为int
   ```

   因为i是一个左值，所以m的类型被推导为int&，auto被推导为int&，这其中用到了引用折叠的规则。而5是一个右值，因此j的类型被推导为int&&，auto被推导为int。

4. 使用auto声明变量，如果==目标对象是一个数组或者**函数**==，则auto会被推导为==对应的指针类型==：

   ```C++
   int i[5];
   auto m = i;    // auto推导类型为int*
   int sum(int a1, int a2)
   {
       return a1+a2;
   }
   auto j = sum   // auto推导类型为int (__cdecl *)(int,int)
   ```

   根据规则4，虽然i是数组类型，但是m会被推导退化为指针类型，同样，j也退化为函数指针类型。

5. 当==auto关键字与列表初始化组合时==，这里的规则有新老两个版本，这里只介绍新规则（C++17标准）。

   1. 直接使用列表初始化，列表中必须为单元素，否则无法编译，auto类型被推导为单元素的类型。
   2. 用等号加列表初始化，列表中可以包含单个或者多个元素，auto类型被推导为std::initializer_list<T>，其中T是元素类型。请注意，在列表中包含多个元素的时候，元素的类型必须相同，否则编译器会报错。

   ```C++
   auto x1 = { 1, 2 };      // x1类型为 std::initializer_list<int>
   auto x2 = { 1, 2.0 };    // 编译失败，花括号中元素类型不同
   auto x3{ 1, 2 };         // 编译失败，不是单个元素
   auto x4 = { 3 };         // x4类型为std::initializer_list<int>
   auto x5{ 3 };            // x5类型为int







思考：

```C++
class Base {
public:
  virtual void f()
  {
      std::cout << "Base::f()" << std::endl;
  };
};
class Derived : public Base {
public:
  virtual void f() override
  {
      std::cout << "Derived::f()" << std::endl;
  };
};
Base* d = new Derived();
auto b = *d; // 或auto& b = *d;
b.f();
```

由于auto b = *d这一句代码是按值赋值的，因此auto会直接推导为Base。代码自然会调用Base的复制构造函数，也就是说Derived被切割成了Base，这里的b.f()最终调用Base的f函数。



对于auto& b = *d语句，会保留cv属性（但此例中没有），auto被推导为Base类型，b推导为Base&类型，从而触发多态调用。



# 返回类型推导（C++14)

> review:尾置返回类型(C++11)：
>
> ```C++
> auto sum(int x, int y)->int {
>     return x + y;
> }
> ```



C++14标准支持对返回类型声明为auto的推导，例如：

```C++
auto sum(int a1, int a2) { return a1 + a2; }
```



# lambda形参使用auto类型推导（C++14）

在C++14标准中我们还可以把auto写到lambda表达式的形参中，这样就得到了一个==泛型的lambda表达式==，例如：

```C++
auto l = [](auto a1, auto a2) { return a1 + a2; };
auto retval = l(5, 5.0);
```

在上面的代码中a1被推导为int类型，a2被推导为double类型，返回值retval被推导为double类型。



lambda表达式返回auto引用：

```C++
auto l = [](int &i)->auto& { return i; };
auto x1 = 5;
auto &x2 = l(x1);
assert(&x1 == &x2);    // 有相同的内存地址
```



# 非类型模板形参占位符(C++17)

C++17标准对auto关键字又一次进行了扩展，使它可以作为==非类型模板形参（即参数模板形参）==的占位符。当然，我们必须保证推导出来的类型是可以用作模板形参的，否则无法通过编译，例如：

```C++
#include 
template<auto N>
void f()
{
  std::cout << N << std::endl;
}
int main()
{
  f<5>();     // N为int类型
  f<'c'>();   // N为char类型
  f<5.0>();   // 编译失败，模板参数不能为double
}
```

在上面的代码中，函数f<5>()中5的类型为int，所以auto被推导为int类型。同理，f<'c'>()的auto被推导为char类型。由于f<5.0>()的5.0被推导为double类型，但是模板参数不能为double类型，因此导致编译失败。



