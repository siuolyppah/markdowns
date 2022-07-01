# 全局变量检测增强

```C
#include <stdio.h>

int a = 10;		// 视为定义
int a;			// 视为声明

int main(){
    printf("%d",a);

    return 0;
}
```



在C编译下通过，但在C++下编译失败



# C++的变量和函数必须有类型

在C语言中：

```C
// i 可以为任意类型c
void fun1(i){
    printf("%d",i);
}

// 没有写参数，可以传任何类型的实参
void fun2(){
}

```



# 严格的类型转换

在C++中，不同类型的变量，一般不能直接赋值，需要进行强制转换。



# C++对结构体的增强

- c中定义结构体变量需要加上struct关键字，c++不需要  
- c中的结构体只能定义成员变量，不能定义成员函数。c++即可以定义成员变量，也可以定义成员函数  



C：

```C
struct stu
{
    int num;
    char name[32];
    
    /* c语言 不允许在结构体中 定义成员函数
    void func(void){
        printf("我是结构体中的func");
	}
     */
};

void test01()
{
    //C语言必须加struct
    struct stu lucy = {100, "lucy"};
}
```



C++：

```C++
struct stu {
    int num;
    char name[32];

    void func() {

    }
};


int main() {
    stu oneStudent = {100, "lucy"};

    return 0;
}
```



# 新增布尔类型

- bool类型占用 1个字节
- 给bool类型赋值时，非0自动转换为true



> C语言：C99标准下，包含头文件stdbool.h，即可使用bool类型





# 增强三目运算符

- c语言三目运算表达式返回值为数据值，为右值，不能赋值  

  ```c
  void test02()
  {
      int a = 10;
      int b = 20;
      printf("C语言:%d\n", a>b?a:b);//20
      
      //a>b?a:b整体结果 右值（不能被赋值）
      a>b?a:b = 100;//err不能被赋值
  }
  ```

- c++语言三目运算表达式返回值为变量本身(引用)，为左值，可以赋值  

  ```C++
  void test02()
  {
      int a = 10;
      int b = 20;
      cout<<"c++中:"<<(a>b?a:b)<<endl;
      
      //a>b?a:b整体结果是变量本身(引用) 左值（能被赋值）
      a>b?a:b = 100;//b =100
  }
  ```

  

>- 左值：Lvalue，L代表Location，表示可内存寻址，可被赋值。
>- 右值：Rvalue，R代表Read，不可被赋值



# Const

## C语言中的Const

- 在C语言中，被Const修饰的变量为**只读变量**

- Const修饰全局变量时，该变量是外部链接的

  > 外部链接，其他源文件可以引入使用



- Const修饰的变量是不可修改的，但由于为该变量开辟了空间，可以通过访问地址进行间接修改

  > 当然，如果是全局只读变量，是不能被修改的。
  >
  > > 它会被存储在文字常量区，是只读的

  ```C
  int main() {
      const int num = 100;
  
      int *p = (int *) &num;
      *p = 200;
  
      printf("%d", num);	// 200
      return 0;
  }
  ```

  

## C++中的Const

- 在C++中，被Const修饰的变量为**常量**

- Const修饰全局变量时，该变量是**内部链接**的

  >extern const ... ，代表该全局常量是外部链接的

- 在C++中，常量视情况创建内存空间：

  - 基础数据类型：

    - 使用字面值初始化：编译器会将其放入符号表中，不分配内存

      当对其取地址时（或定义为extern），将分配内存，并将指针指向该区域

      ```C++
      const int a = 10;	
      int* p = (int *)&a;	// p指向临时分配的空间；a仍存储在符号表中
      ```

    - 使用变量初始化：直接开辟空间，意味着临时const变量此时可修改

      ```C++
      int main() {
          int num = 100;
          const int a = num;
          int *p = (int *) &a;
      
          *p = 200;
      
          cout << a << endl;  // 200
      
          return 0;
      }
      ```

    - 自定义类型（如类、结构体）：直接开辟内存

  

  ```C++
  
  int main() {
      const int num = 100;
  
      int *p = (int *) &num;
      *p = 200;
  
      cout << num << endl;    // 100
      cout << *p << endl;     // 200
      return 0;
  }
  ```



## 总结

相同之处：

- 都不可以直接被Const修饰的变量



区别：

- C：可以通过访问地址进行修改（前提是局部const变量）
- C++：
  - 使用字面值初始化的基本数据类型：不可修改，通过地址访问的是临时内存空间
  - 其他情况：可通过地址修改（前提是局部const变量）



## 尽量以const代替#define

- 宏变量没有类型
- 宏变量的作用域：从定义处，到文件结束（或 #undef 卸载处）



# Reference

- 引用，可以作为一个已定义变量的别名  

  ```c++
  int a = 100;
  int &b = a;
  ```

- 引用必须初始化

- 引用不可改变，指向其他其他变量

- 不能有NULL引用



## 引用作用于数组

- 使用()改变优先级：

  ```c++
  int main() {
      int arr[5] = {0, 1, 2, 3, 4};
  
  //    int &arrRef[5] = arr;  类型为：5个int&组成的数组
      int (&arrRef)[5] = arr; // ok
  
      return 0;
  }
  ```

- 为数组起别名：

  ```C++
  typedef int MyArr[5];
  
  int main() {
      int arr[5] = {0, 1, 2, 3, 4};
  
      MyArr &arrRef = arr;
      cout << arrRef[3] << endl;
  
      return 0;
  }
  ```

  

## 引用作为函数返回值

当函数返回值时， 引用关联的内存一定要存在。  

- 可以返回静态变量引用；但**不能返回局部变量的引用**
- 如果函数做左值， 那么必须返回引用  