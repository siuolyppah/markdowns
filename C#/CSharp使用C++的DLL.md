> 演示代码见案例



# C++项目设置

1. 设置CLR：

   ![image-20220731221906211](CSharp%E4%BD%BF%E7%94%A8C++%E7%9A%84DLL.assets/image-20220731221906211.png)

2. 设置命令行参数：
   ![image-20220731221919418](CSharp%E4%BD%BF%E7%94%A8C++%E7%9A%84DLL.assets/image-20220731221919418.png)



- Adder.h:

  ```C++
  #ifndef ADDER_H
  #define ADDER_H
  
  #include<iostream>
  using namespace std;
  
  class Adder {
  public:
  	int add(int a, int b) {
  		return a + b;
  	}
  
  	Adder() {
  		cout << "Adder()" << endl;
  	}
  
  	~Adder() {
  		cout << "~Adder()" << endl;
  	}
  
  };
  
  #endif // !ADDER_H
  ```

- Adder.cpp:

  ```C++
  #include "pch.h"
  
  #include "Adder.h"
  
  extern "C" __declspec(dllexport)
  int doubleIt(int x) {
  	return x * 2;
  }
  
  extern "C" __declspec(dllexport)
  Adder * establishAdder() {
  	Adder* ptr = new Adder();
  
  	return ptr;
  }
  
  extern "C" __declspec(dllexport)
  void freeAdder(Adder * ptr) {
  	delete ptr;
  }
  
  extern "C" __declspec(dllexport)
  int adder_add(Adder* ptr,int a, int b) {
  	return ptr->add(a, b);
  }
  ```

  





# C#项目设置

添加项目引用即可：

![image-20220731222000338](CSharp%E4%BD%BF%E7%94%A8C++%E7%9A%84DLL.assets/image-20220731222000338.png)

program.cs:

```cs
using System.Runtime.InteropServices;

namespace Client
{
    class Program
    {

        [DllImport("Hello.dll")]
        extern static int doubleIt(int x);

        [DllImport("Hello.dll")]
        extern static IntPtr establishAdder();

        [DllImport("Hello.dll")]
        extern static void freeAdder(IntPtr ptr);

        [DllImport("Hello.dll")]
        extern static int adder_add(IntPtr p, int x, int y);

        static void Main(String[] args)
        {
            int x = doubleIt(2);
            Console.WriteLine(x);


            IntPtr ptr = establishAdder();
            int y = adder_add(ptr, 1, 2);
            Console.WriteLine(y);
            freeAdder(ptr);
        }
    }
}
```

