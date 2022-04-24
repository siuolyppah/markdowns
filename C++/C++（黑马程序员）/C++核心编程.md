# 内存分区模型

C++程序在执行时，将内存大方向划分为**4个区域**

- 代码区：存放函数体的二进制代码，由操作系统进行管理的
- 全局区：存放全局变量和静态变量以及常量
- 栈区：由编译器自动分配释放, 存放函数的参数值,局部变量等
- 堆区：由程序员分配和释放,若程序员不释放,程序结束时由操作系统回收



## 程序运行前（代码区与全局区）

在程序编译后，生成了exe可执行程序，**未执行该程序前**分为两个区域：

- **代码区：**存放 CPU 执行的机器指令

  - 代码区是**共享**的，共享的目的是对于频繁被执行的程序，只需要在内存中有一份代码即可
  - 代码区是**只读**的，使其只读的原因是防止程序意外地修改了它的指令

- **全局区**：用于存放：

  - ***全局变量***
  - 静态变量
  - 常量区：字符串常量、其他常量（const修饰的**全局变量**）

  >该区域的数据在程序结束后***<u>由操作系统释放</u>***



```C++
int g_a, g_b;
const int c_g_a = 0,c_g_b = 0;

int main() {
    int a, b;
    //局部变量
    cout << (long long) &a << endl;     //187590245596
    cout << (long long) &b << endl;     //187590245592

    //全局变量
    cout << (long long) &g_a << endl;   //140694909714496
    cout << (long long) &g_b << endl;   //140694909714500

    //静态变量
    static int s_a, s_b;
    cout << (long long) &s_a << endl;   //140694909714508
    cout << (long long) &s_b << endl;   //140694909714512

    //字符串常量
    cout << (long long) &"A" << endl;  //140694909702156
    cout << (long long) &"B" << endl;  //140694909702158

    //const修饰的全局变量
    cout << (long long) &c_g_a << endl;   //140694909702148
    cout << (long long) &c_g_b << endl;   //140694909702152


    //const修饰的局部变量
    const int c_a = 0,c_b = 0;
    cout << (long long) &c_a << endl;   //187590245588
    cout << (long long) &c_b << endl;   //187590245584

    return 0;
}
```



## 程序执行时（栈区与堆区）

- 栈区：由编译器自动分配释放, 存放函数的参数值(形参）,局部变量等

  ```c++
  int * func(){
      int a = 10;
      return &a;
  }
  
  int main() {
      int *p = func();
      cout << *p << endl;	//第一次ok，由编译器支持
      cout << *p << endl;	//野指针，指向了实际被释放的地址
      
      return 0;
  }
  ```

  > 不要返回局部变量的地址，***栈区开辟的数据将由编译器自动释放***

- 堆区：由程序员分配释放。若程序员不释放,程序结束时由操作系统回收

  ```C++
  int* func(){
  	int* a = new int(10);
  	return a;
  }
  
  int main() {
  	int *p = func();
  	cout << *p << endl;
      
  	return 0;
  }
  ```

  >在C++中主要利用new在堆区开辟内存

  

## new与delete操作符

- C++中利用==new==操作符在堆区开辟内存，利用==delete==释放内存

- 利用new创建的数据，***会返回该数据对应的类型的指针***
- 语法： `new 数据类型`



用法：

- 基本用法：

  ```c++
  int* func(){
      int* a = new int(10);
      return a;
  }
  
  int main() {
      int *p = func();
      cout << *p << endl;
  
      //利用delete释放堆区数据
      delete p;
      //cout << *p << endl; //报错，释放的空间不可访问
  
      return 0;
  }
  ```

- 开辟数组：

  ```c++
  //堆区开辟数组
  int main() {
  
      int* arr = new int[10];
  
      for (int i = 0; i < 10; i++)
      {
          arr[i] = i + 100;
      }
  
      //释放数组 delete 后加 []
      delete[] arr;
  
      return 0;
  }
  ```



# 引用

## 引用的基本使用

- **作用： **给变量起别名
- **语法：** `数据类型 &别名 = 原名`



```c++
int main() {
    int a = 10;
    int &b = a;

    cout << "a = " << a << endl;	//10
    cout << "b = " << b << endl;	//10

    b = 100;

    cout << "a = " << a << endl;	//100
    cout << "b = " << b << endl;	//100

    return 0;
}
```



## 引用注意事项

* 引用必须初始化
* 引用在初始化后，不可以改变

```c++
int main() {

    int a = 10;
    int b = 20;
    //int &c; //错误，引用必须初始化
    int &c = a; //一旦初始化后，就不可以更改
    c = b; //这是赋值操作，不是更改引用

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;

    return 0;
}
```



## 引用做函数参数

**作用：**函数传参时，可以利用引用的技术让形参修饰实参

**优点：**可以***简化指针修改实参***

> 即通过引用参数产生的效果同按地址传递是一样的。引用的语法更清楚简单

```c++
//1. 值传递，无效
void mySwap01(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

//2. 地址传递
void mySwap02(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//3. 引用传递
void mySwap03(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {

    int a = 10;
    int b = 20;

    mySwap01(a, b);
    cout << "a:" << a << " b:" << b << endl;

    mySwap02(&a, &b);
    cout << "a:" << a << " b:" << b << endl;

    mySwap03(a, b);
    cout << "a:" << a << " b:" << b << endl;
    
    return 0;
}
```



## 引用做函数返回值

- 作用：引用是可以作为函数的返回值存在的

- 注意：**不要返回局部变量引用**

- 用法：函数调用作为左值



```c++
//返回局部变量引用
int& test01() {
    int a = 10; //局部变量
    return a;
}

//返回静态变量引用
int& test02() {
    static int a = 20;
    return a;
}

int main() {

    //不能返回局部变量的引用
    int& ref = test01();
    cout << "ref = " << ref << endl;
    cout << "ref = " << ref << endl;

    int& ref2 = test02();
    cout << "ref2 = " << ref2 << endl;
    cout << "ref2 = " << ref2 << endl;

    //如果函数的返回值是引用，则该函数调用可作为左值
    test02() = 1000;

    cout << "ref2 = " << ref2 << endl;
    cout << "ref2 = " << ref2 << endl;

    return 0;
}
```



## 引用的本质

本质：**引用的本质在c++内部实现是一个指针常量.**（xxx* const p，即不能改变指向）

```c++
//发现是引用，转换为 int* const ref = &a;
void func(int& ref){
    ref = 100; // ref是引用，转换为*ref = 100
}
int main(){
    int a = 10;

    //自动转换为 int* const ref = &a; 
    int& ref = a; 
    ref = 20; //内部发现ref是引用，自动帮我们转换为: *ref = 20;

    cout << "a:" << a << endl;
    cout << "ref:" << ref << endl;

    func(a);
    return 0;
}
```



## 常量引用

- **作用：**常量引用主要用来修饰形参，防止误操作

  >在函数形参列表中，可以加==const修饰形参==，防止形参改变实参

  

```c++
//引用使用的场景，通常用来修饰形参
void showValue(const int& v) {
    //v += 10;
    cout << v << endl;
}

int main() {

    //int& ref = 10;  引用本身需要一个合法的内存空间，因此这行错误
    //加入const就可以了，编译器优化代码，int temp = 10; const int& ref = temp;
    const int& ref = 10;
    //ref = 100;  //加入const后不可以修改变量

    int a = 10;
    showValue(a);
    
    return 0;
}
```



# 函数提高

## 函数的默认参数

在C++中，函数的形参列表中的形参是可以有默认值的。

语法：` 返回值类型  函数名 （参数= 默认值）{}`

***默认参数，可以不传递***



但注意：

- 如果某个位置参数有默认值，那么***从这个位置往后（即从左向右），必须都要有默认值***
- ***如果函数声明有默认值，函数实现的时候就不能有默认参数***

```c++
int func(int a, int b = 10, int c = 10) {
    return a + b + c;
}

int func2(int a = 10, int b = 10);
int func2(int a, int b) {
    return a + b;
}

int main() {

    cout << "ret = " << func(20, 20) << endl;
    cout << "ret = " << func(100) << endl;

    system("pause");

    return 0;
}
```



## 函数占位参数

C++中函数的形参列表里可以有占位参数，用来做占位，调用函数时必须填补该位置

**语法：** `返回值类型 函数名 (数据类型){}`

注意，***占位参数也可以设置默认参数***



```c++
//函数占位参数 ，占位参数也可以有默认参数
void func(int a, int, double = 1.2) {
    cout << "this is func" << endl;
}

int main() {
    func(10,10); //占位参数必须填补
    
    return 0;
}
```



## 函数重载

**作用：**函数名可以相同，提高复用性



**函数重载满足条件：**

* 同一个作用域下
* 函数名称相同
* 函数参数**类型不同**  或者 **个数不同** 或者 **顺序不同**（即形参表不同）

> ***函数的返回值不可以作为函数重载的条件***

```c++
//函数重载需要函数都在同一个作用域下
void func(){}
void func(int a){}
void func(double a){}
void func(int a ,double b){}
void func(double a ,int b){}

//函数返回值不可以作为函数重载条件
//int func(double a, int b)
//{
//	cout << "func (double a ,int b)的调用！" << endl;
//}


int main() {

    func();
    func(10);
    func(3.14);
    func(10,3.14);
    func(3.14 , 10);

    system("pause");

    return 0;
}
```



函数重载的注意事项：

- 引用作为重载条件

  ```c++
  void func(int &a) {
      cout << "int&" << endl;
  }
  
  void func(const int &a) {
      cout << "const int&" << endl;
  }
  
  int main() {
      int x1 = 100;
      const int x2 = 100;
  
      func(x1);   //int&
      func(x2);   //const int&
      func(10);   //const int&
  
      return 0;
  }
  ```

  > 因为本质上：
  >
  > - int&a 等同于 int* const a；
  > - 而const int &a 等同于 const int* const a

- 函数重载碰到函数默认参数

  ```c++
  void func(int a) {
      cout << "a" << endl;
  }
  
  void func(int a, int b = 10) {
      cout << "a,b" << endl;
  }
  
  int main() {
      func(10);//Error:Call to 'func' is ambiguous
  
      return 0;
  }
  ```



# 类与对象

[黑马程序员匠心之作|C++教程从0到1入门编程,学习编程不再难_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1et411b73Z?p=99)