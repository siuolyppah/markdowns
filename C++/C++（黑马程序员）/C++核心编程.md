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

## 类的定义

> 属性和行为，都是类的***成员***



```c++
const double PI = 3.1415926;

class Circle {
public:
    int r;

    double getPerimeter() {
        return 2 * PI * r;
    }
};

int main() {

    Circle circle;
    circle.r = 10;
    cout << circle.getPerimeter() << endl;


    return 0;
}
```



## 封装

访问权限有三种：

| 访问权限修饰符 |   权限   |                             说明                             |
| :------------: | :------: | :----------------------------------------------------------: |
|     public     | 公共权限 |                  类内可以访问，类外可以访问                  |
|   protected    | 保护权限 | 类内可以访问，类外不可以访问。<br />子类可访问父类的保护权限 |
|    private     | 私有权限 | 类内可以访问，类外不可以访问<br />子类不可访问父类的私有权限 |

 

```c++
class Person {
public:
    string name;
protected:
    string car;
private:
    string account;

public:
    void func() {
        name = "小明";
        car = "祖传车";
        account = "abcdef";
    }
};

int main() {

    Person person;

    cout << person.name << endl;
    cout << person.car << endl;        //error
    cout << person.account << endl;     //error
    
    return 0;
}
```



### struct和class的区别

区别在于：***默认的访问权限不同***：

- struct：默认权限为public
- class：默认权限为privaate



```c++
class C1
{
	int  m_A; //默认是私有权限
};

struct C2
{
	int m_A;  //默认是公共权限
};

int main() {

	C1 c1;
	c1.m_A = 10; //错误，访问权限是私有

	C2 c2;
	c2.m_A = 10; //正确，访问权限是公共
    
	return 0;
}
```



## 对象的初始化和清理

### 构造函数和析构函数

- **构造函数**和**析构函数**会被被编译器自动调用，完成对象初始化和清理工作
- 如果不提供构造函数和析构函数，则由编译器默认提供（空实现）



- 构造函数：

  创建对象时为对象的成员属性赋值，构造函数由编译器自动调用，无须手动调用

- 析构函数：

  在对象**销毁前**系统自动调用，执行一些清理工作



**构造函数语法：**`类名(){}`

1. 构造函数，没有返回值也不写void
2. 函数名称与类名相同
3. 构造函数可以有参数，因此***可以发生重载***
4. 程序在调用对象时候会自动调用构造，无须手动调用,而且只会调用一次



**析构函数语法：** `~类名(){}`

1. 析构函数，没有返回值也不写void
2. 函数名称与类名相同,在名称前加上符号  `~`
3. ***析构函数不可以有参数，因此不可以发生重载***
4. 程序在对象销毁前会自动调用析构，无须手动调用,而且只会调用一次



```c++
class Person
{
public:
    //构造函数
    Person()
    {
        cout << "Person的构造函数调用" << endl;
    }
    
    //析构函数
    ~Person()
    {
        cout << "Person的析构函数调用" << endl;
    }

};

void test()
{
    Person p;
}

int main() {

    test();
    
    return 0;
}
```



### 构造函数的分类及调用

构造函数可分为：

- 按参数分为： 有参构造（***即默认构造函数***）和无参构造

- 按类型分为： 普通构造和***拷贝构造***（要求形参有const和&限定）

  ```c++
  class Person {
  public:
      int age;
  
      //普通构造函数
      Person() {}
  
      //拷贝构造函数
      Person(const Person &person) {
          age = person.age;
      }
  
  };
  ```



三种调用方式：

- 括号法

  > 调用默认构造函数，不要带小括号。否则将会被编译器视为方法的声明。
  >
  > ```c++
  > int main(){
  >     //返回值类型为Person，方法名为p1
  >     Person p1();	
  > }
  > ```

- 显式法

  > - 匿名对象：没有标识符的对象，当前行执行结束后，匿名对象将被回收
  >
  > - 不能用拷贝构造函数，初始化匿名对象
  >
  >   ```c++
  >   int main(){
  >       Person p1;
  >       Person (p1);//ERROR，编译器会将其视为Person p1;
  >   }
  >   ```
  >
  >   

- 隐式转换法

```c++
class Person {
public:
    int age;

    Person() {
        cout << "()" << endl;
    }

    Person(int age) {
        cout << "(int)" << endl;
    }

    Person(const Person &person) {
        age = person.age;
        cout << "(const Person&)" << endl;
    }

};

int main() {

    //括号法
    Person p1;              //默认构造函数
    Person p2(10);
    Person p3(p2);          //拷贝构造函数

    //显式法
    Person p4;
    Person p5 = Person(10);
    Person p6 = Person(p5);

    //隐式转换法
    Person p7 = 10; //等价于Person p7 = Person(10);
    Person p8 = p7; //等价于Person p8 = Person(p7);

    return 0;
}
```



### 拷贝构造函数调用时机

```c++
class Person {
public:
    Person() {
        cout << "()" << endl;
    }

    Person(const Person &person) {
        cout << "const Person&" << endl;
    }

    ~Person() {
        cout << "~" << endl;
    }
};
```



C++中拷贝构造函数调用时机通常有三种情况：

* 使用一个已经创建完毕的对象来初始化一个新对象

  ```c++
  //使用已存在的对象，初始化新对象
  void test1(){
      Person p1;
      Person p2(p1);
  }
  ```

* **值传递**的方式给函数参数传值

  ```c++
  //形参p的创建，由拷贝构造而来
  void test2(Person p){
  }
  
  int main(){
      Person p1;
      test2(p1);
  
      return 0;
  }
  ```

* 以**值传递**返回局部对象

  ```c++
  //p1变量本身会在函数执行完被释放；但函数将返回它的拷贝构造
  Person test3(){
      Person p1;
      return p1;
  }
  ```

  

### 函数默认提供规则

c++编译器至少给一个类添加3个函数：

​	1．默认构造函数(无参，函数体为空)

​	2．默认析构函数(无参，函数体为空)

​	3．默认拷贝构造函数，对属性进行**值拷贝**(浅拷贝)



同时存在如下规则：

- 当用户编写有参构造函数时，不再默认提供无参构造，但仍存在默认拷贝构造
- 当用户编写拷贝构造函数时，不再提供构造函数。



```c++
class Person {
public:
    //无参（默认）构造函数
    Person() {
        cout << "无参构造函数!" << endl;
    }
    //有参构造函数
    Person(int a) {
        age = a;
        cout << "有参构造函数!" << endl;
    }
    //拷贝构造函数
    Person(const Person& p) {
        age = p.age;
        cout << "拷贝构造函数!" << endl;
    }
    //析构函数
    ~Person() {
        cout << "析构函数!" << endl;
    }
public:
    int age;
};

void test01()
{
    Person p1(18);
    //如果不写拷贝构造，编译器会自动添加拷贝构造，并且做浅拷贝操作
    Person p2(p1);

    cout << "p2的年龄为： " << p2.age << endl;
}

void test02()
{
    //如果用户提供有参构造，编译器不会提供默认构造，会提供拷贝构造
    Person p1; //此时如果用户自己没有提供默认构造，会出错
    Person p2(10); //用户提供的有参
    Person p3(p2); //此时如果用户没有提供拷贝构造，编译器会提供

    //如果用户提供拷贝构造，编译器不会提供其他构造函数
    Person p4; //此时如果用户自己没有提供默认构造，会出错
    Person p5(10); //此时如果用户自己没有提供有参，会出错
    Person p6(p5); //用户自己提供拷贝构造
}

int main() {

    test01();

    system("pause");

    return 0;
}
```



### 深拷贝与浅拷贝

- 浅拷贝：简单的赋值拷贝操作

  浅拷贝的隐患：

  - 重复释放堆区

- 深拷贝：在堆区重新申请空间，进行拷贝操作


>如果属性有在堆区开辟的，一定要自己提供拷贝构造函数，防止浅拷贝带来的问题



- 不同对象的成员，指向了同一堆区：

  ```c++
  class Person {
  public:
      int age;
      int *ptr_height;
  
  public:
      Person() {}
  
      Person(int age, int height) {
          this->age = age;
          this->ptr_height = new int(height);
      }
      ~Person() {
          delete this->ptr_height;
          ptr_height = nullptr;
      }
  };
  
  
  int main() {
  
      Person p1(18, 180);
      cout << p1.ptr_height << endl;  //0x1ce6c5718d0
  
      Person p2(p1);
      cout << p2.ptr_height << endl;  //0x1ce6c5718d0
  
      return 0;
  }
  ```

  解决方案：重写默认的拷贝构造函数；

  ```c++
  Person(const Person &person) {
      this->age = person.age;
      this->ptr_height = new int(*person.ptr_height);
  }
  ```



### 初始化列表

**语法：**`构造函数()：属性1(值1),属性2（值2）... {}`

```c++
class Person {
public:

	////传统方式初始化
	//Person(int a, int b, int c) {
	//	m_A = a;
	//	m_B = b;
	//	m_C = c;
	//}

	//初始化列表方式初始化
	Person(int a, int b, int c) :m_A(a), m_B(b), m_C(c) {}

private:
	int m_A;
	int m_B;
	int m_C;
};

int main() {
	Person p(1, 2, 3);
	return 0;
}
```



### 类对象作为类成员

C++类中的成员可以是另一个类的对象，我们称该成员为 ***对象成员***



例如：B类中有对象A作为成员，A为对象成员

```c++
class A {
public:
    A() {
        cout << "A构造" << endl;
    }
    ~A(){
        cout << "A析构" << endl;
    }
};

class B {
public:
    A a;

    B(){
        cout << "B构造" << endl;
    }
    ~B(){
        cout << "B析构" << endl;
    }
};


int main() {
    B b;
    /* A构造
     * B构造
     * B析构
     * A析构
     * */

    return 0;
}
```

> 即先调用对象成员的构造函数；再调用本类构造



## 静态成员

静态成员就是在成员变量和成员函数前加上**关键字static**，称为静态成员

静态成员分为：

- 静态成员变量

  *  所有对象共享同一份数据
  *  在编译阶段分配内存
  *  类内声明，类外初始化

  ```c++
  class Person {
  public:
      static int a;
  };
  //类外初始化
  int Person::a = 10;
  ```

- 静态成员函数

  *  所有对象共享同一个函数
  *  静态成员函数只能访问静态成员变量

  ```c++
  class Person {
  public:
      static int a;
      int b;
  
      static void func() {
          a = 20;
          b = 20; //ERROR
      }
  };
  int Person::a = 10;
  
  int main() {
  
      Person p;
      //静态成员函数的调用：
      p.func();
      Person::func();
  
      return 0;
  }
  ```

  

## C++对象模型与this指针

[黑马程序员匠心之作|C++教程从0到1入门编程,学习编程不再难_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1et411b73Z?p=114&spm_id_from=pageDriver)

### 成员变量和成员函数分开存储

- ***只有非静态的成员变量，才分配在类的对象上***
- 静态成员变量在全局区；成员函数都分配在代码区

- 特别的，一个没有成员的对象，占用1个字节。（用于区分两个空对象）



### this指针

- ***this指针，指向调用此成员函数的对象***。

- this指针，隐含在每一个非静态成员函数内



this指针的用途：

- 当形参与成员变量同名时，可用this指针进行区分
- 在类的非静态成员函数中，返回调用者本身（即return *this）。



***this指针的本质：指针常量，即指向不可变***



### 空指针访问成员函数

在C++中，***空指针，可以调用成员函数***

```c++
class Person{
public:
    int a;

    void doThing(){
        cout<<"nothing done!"<<endl;
    }

    void coutA(){
        cout<< this->a <<endl;
    }
};

int main() {

    Person* ptr = nullptr;
    ptr->doThing();      //ok
    ptr->coutA();       //error

    return 0;
}
```

> 要注意保证代码健壮性



### const修饰成员函数

- 常函数：

  - 即：成员函数的()后，跟有关键字const

  - ***常函数内，不能修改成员属性***

    > 因为此时，this指针变为const Person* const this

  - ***但声明为mutable的成员属性，在常函数中仍可修改***

- 常对象：

  - 即：声明为const的对象
  - ***常对象仅能调用常函数，仅能修改mutable属性***



```c++
class Person {
public:
    int a;
    mutable int b;

    void changePerson(){
        this->a=10;
    }

    void showPerson() const {
        //this->a =10;    //ERROR
        this->b = 20;
    }
};

int main() {
    Person p1;
    p1.showPerson();
    p1.changePerson();

    Person const p2{10,20};
    p2.showPerson();
    //p2.changePerson();    //ERROR
    //p2.a=100;             //ERROR
    p2.b=100;

    return 0;
}
```



## 友元

[黑马程序员匠心之作|C++教程从0到1入门编程,学习编程不再难_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1et411b73Z?p=118&spm_id_from=pageDriver)