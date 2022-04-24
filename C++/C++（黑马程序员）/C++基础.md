# 数组

## 一维数组

一维数组定义的三种方式：

1. ` 数据类型  数组名[ 数组长度 ]; `
2. `数据类型  数组名[ 数组长度 ] = { 值1，值2 ...};`
3. `数据类型  数组名[ ] = { 值1，值2 ...};`



一维数组的数组名：

- 可以统计整个数组在内存中的长度

  ```c++
  int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
  cout << "整个数组所占内存空间为： " << sizeof(arr) << endl;
  cout << "每个元素所占内存空间为： " << sizeof(arr[0]) << endl;
  cout << "数组的元素个数为： " << sizeof(arr) / sizeof(arr[0]) << endl;
  ```

- 可以获取数组在内存中的首地址

  ```c++
  cout << "数组首地址为： " << (int)arr << endl;
  cout << "数组中第一个元素地址为： " << (int)&arr[0] << endl;
  cout << "数组中第二个元素地址为： " << (int)&arr[1] << endl;
  ```



## 二维数组

二维数组定义的四种方式：

1. ` 数据类型  数组名[ 行数 ][ 列数 ]; `
2. `数据类型  数组名[ 行数 ][ 列数 ] = { {数据1，数据2 } ，{数据3，数据4 } };`
3. `数据类型  数组名[ 行数 ][ 列数 ] = { 数据1，数据2，数据3，数据4};`
4. ` 数据类型  数组名[  ][ 列数 ] = { 数据1，数据2，数据3，数据4};`



二维数组的数组名：

* 查看二维数组所占内存空间

  ```c++
  int arr[2][3] =
  {
      {1,2,3},
      {4,5,6}
  };
  
  cout << "二维数组大小： " << sizeof(arr) << endl;
  cout << "二维数组一行大小： " << sizeof(arr[0]) << endl;
  cout << "二维数组元素大小： " << sizeof(arr[0][0]) << endl;
  
  cout << "二维数组行数： " << sizeof(arr) / sizeof(arr[0]) << endl;
  cout << "二维数组列数： " << sizeof(arr[0]) / sizeof(arr[0][0]) << endl;
  ```

* 获取二维数组首地址

  ```c++
  cout << "二维数组首地址：" << arr << endl;
  cout << "二维数组第一行地址：" << arr[0] << endl;
  cout << "二维数组第二行地址：" << arr[1] << endl;
  
  cout << "二维数组第一个元素地址：" << &arr[0][0] << endl;
  cout << "二维数组第二个元素地址：" << &arr[0][1] << endl;
  ```

  



# 函数

- C++的参数传递，为***值传递***

- 函数可以声明多次，但只能定义一次

  ```c++
  //声明
  int max(int a, int b);
  int max(int a, int b);
  
  //定义
  int max(int a, int b)
  {
  	return a > b ? a : b;
  }
  ```



## 函数的分文件编写

**作用：**让代码结构更加清晰



函数分文件编写一般有4个步骤：

1. 创建后缀名为.h的头文件  
2. 创建后缀名为.cpp的源文件
3. ***在头文件中写函数的声明***
4. ***在源文件中写函数的定义***



**示例：**

```C++
//swap.h文件

#ifndef C__LEARN_SWAP_H
#define C__LEARN_SWAP_H

void swap(int &a, int &b);

#endif //C__LEARN_SWAP_H
```

```C++
//swap.cpp文件

#include "swap.h"
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}
```

```C++
//main函数文件

#include <iostream>
#include "swap.h"

int main() {
    int a = 10;
    int b = 20;

    swap(a, b);
    std::cout << a <<' '<<b << std::endl;

    return 0;
}
```



# 指针

## const修饰的指针

const修饰指针有三种情况

1. const修饰指针   --- ***常量指针***

   ```c++
   const int * p =&a;
   //const 修饰的 int*，即指针指向的值不可变
   ```

2. const修饰常量   --- ***指针常量***

   ```c++
   int * const p = &a;
   //const修饰的是p，即指针不可变
   ```

3. const即修饰指针，又修饰常量

>技巧：看const右侧紧跟着的是指针还是常量, 是指针就是常量指针，是常量就是指针常量



```C++
int main() {

    int a = 10;
    int b = 10;

    //const修饰的是指针，指针指向可以改，指针指向的值不可以更改
    const int * p1 = &a; 
    p1 = &b; //正确
    //*p1 = 100;  报错

    //const修饰的是常量，指针指向不可以改，指针指向的值可以更改
    int * const p2 = &a;
    //p2 = &b; //错误
    *p2 = 100; //正确

    //const既修饰指针又修饰常量
    const int * const p3 = &a;
    //p3 = &b; //错误
    //*p3 = 100; //错误

    system("pause");

    return 0;
}
```



## 指针与数组

利用指针，可以访问数组中元素



```c++
int main() {

	int arr[] = { 1,2,3,4,5,6,7,8,9,10 };

	int * p = arr;  //指向数组的指针

	cout << "第一个元素： " << arr[0] << endl;
	cout << "指针访问第一个元素： " << *p << endl;

	for (int i = 0; i < 10; i++)
	{
		//利用指针遍历数组
		cout << *p << endl;
		p++;
	}

	system("pause");

	return 0;
}
```



## 指针与数组

***当数组名传入到函数作为参数时，被退化为指向首元素的指针***

```c++
void test(int* arr){
    ...
}

void test2(int[] arr){
    
}

int main(){
	int arr[10] = { 4,3,6,9,1,2,10,8,7,5 };
	
	test(arr);
	test2(arr);
    
	return 0;
}
```



# 结构体

**语法：**`struct 结构体名 { 结构体成员列表 }；`



通过结构体***创建变量***的方式有三种：

1. struct 结构体名 变量名
2. struct 结构体名 变量名 = { 成员1值 ， 成员2值...}
3. 定义结构体时顺便创建变量

> struct 关键字可省略



```c++
//结构体定义
struct student
{
    //成员列表
    string name;  
    int age;      
    int score;    
}stu3; //结构体变量创建方式3 

int main() {

    //结构体变量创建方式1
    struct student stu1; //struct 关键字可以省略
    stu1.name = "张三";
    stu1.age = 18;
    stu1.score = 100;

    //结构体变量创建方式2
    struct student stu2 = { "李四",19,60 };

    //结构体变量创建方式3 
    stu3.name = "王五";
    stu3.age = 18;
    stu3.score = 80;

    return 0;
}
```



## 访问结构体成员

使用`.`操作符访问成员



```c++
struct student
{
    string name;  
    int age;      
    int score;    
};

int main() {
    struct student stu = { "张三",18,100, };
    stu.age =20;
}
```



## 结构体指针

当指针p指向结构体变量时，p->age可访问到其age成员。等同于(*p).age

```c++
struct student
{
    string name;  
    int age;      
    int score;    
};

int main() {
    struct student stu = { "张三",18,100, };
    struct student * p = &stu;

    p->age =20;	//等同于 (*p).age=20;
    return 0;
}
```



## 结构体与const

**作用：**用const来防止误操作



```c++
//学生结构体定义
struct student
{
    //成员列表
    string name;  //姓名
    int age;      //年龄
    int score;    //分数
};

//const使用场景
void printStudent(const student *stu) //加const防止函数体中的误操作
{
    //stu->age = 100; //操作失败，因为加了const修饰
    cout << stu->name <<endl;
}

int main() {

    student stu = { "张三",18,100 };
    printStudent(&stu);

    return 0;
}
```

