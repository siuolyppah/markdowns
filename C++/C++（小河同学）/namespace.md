# NameSpace

## 定义

只能在全局作用域中定义



## 嵌套

```c++
#include <iostream>

namespace outer {
    int x = 100;

    namespace inner {
        int y = 200;
    }
}

int main() {

    std::cout << outer::x << std::endl;
    std::cout << outer::inner::y << std::endl;
    
    return 0;
}
```



## 命名空间是开放的

随时可将新的成员，加入已有的命名空间

```C++
#include <iostream>

namespace my_namespace {
    int x = 100;
}

namespace my_namespace {
    int y = 100;
}

int main() {
    std::cout << my_namespace::x << std::endl;
    std::cout << my_namespace::y << std::endl;

    return 0;
}
```



## 命名空间中函数

### 可直接访问命名空间的变量

```c++
namespace my_namespace {
    int x = 100;

    void func(){
        std::cout << x << std::endl;
    }
}

int main() {
    std::cout << my_namespace::x << std::endl;

    my_namespace::func();

    return 0;
}
```



### 声明与定义可分离

```C++
namespace my_namespace {
    int x = 100;

    void func();
}

void my_namespace::func() {
    std::cout << x << std::endl;
}

int main() {
    std::cout << my_namespace::x << std::endl;

    my_namespace::func();

    return 0;
}
```



## 无名命名空间

- 无名命名空间，其中的变量和函数，**仅可在本文件中使用**。
- 等价于，为标识符添加 static 修饰符



```c++
namespace {
    int x = 100;

    void func() {
        std::cout << x << std::endl;
    }
}

int main() {

    std::cout << x << std::endl;
    func();

    return 0;
}
```



## 别名

```C++
namespace my_namespace {
    int x = 100;

    void func() {
        std::cout << x << std::endl;
    }
}

int main() {
    namespace mn = my_namespace;

    std::cout << my_namespace::x << std::endl;
    std::cout << mn::x << std::endl;

    return 0;
}
```



# `using`声明

## using写的位置

可以写在：

- 全局作用域
- 函数作用域



例如：

```C++
#include <iostream>

using namespace std;

namespace myNamespace {
    int x = 100;

    void func() {
        std::cout << x << std::endl;
    }
}

int main() {
    using namespace myNamespace;

    cout << x << endl;
    func();

    return 0;
}
```



## using后的变量查找优先级

1. 先找局部变量
2. 再找namespace
3. 全局变量

```C++
#include <iostream>

using namespace std;

namespace myNamespace {
    int x = 100;
}

int main() {
    int x = 200;
    using namespace myNamespace;

    cout << x << endl;  // 200

    return 0;
}
```



## 写法

- 使用整个命名空间：

  ```c++
  using namespace A;
  ```

  > 使用此方式时，若变量重名，则按优先级查找

- 使用命名空间的某个具体成员：

  ```c++
  using namespace A::a;
  ```

  > 使用此方式时：
  >
  > - 若与局部变量重名，则报错
  >
  > - 若与全局变量重名，则隐藏全局变量：
  >
  >   ```C++
  >   #include <iostream>
  >   
  >   using namespace std;
  >   
  >   namespace myNamespace {
  >       int x = 100;
  >   }
  >   
  >   int x = 200;
  >   
  >   int main() {
  >       using myNamespace::x;
  >   
  >       cout << x << endl;      // 100
  >       cout << ::x << endl;    // 200
  >   
  >       return 0;
  >   }
  >   ```



```c++
#include <iostream>

using namespace std;

namespace myNamespace {
    int x = 100;

    void func() {
        cout << x << endl;
    }
}

int main() {
    using myNamespace::x;
    cout<<x<<endl;

    myNamespace::func();
    
    return 0;
}
```



## using遇到函数重载

将引入所有的重载函数：

```C++
#include <iostream>

using namespace std;

namespace myNamespace {
    void func() {
        cout << "fun()" << endl;
    }

    void func(int x) {
        cout << "fun(int)" << endl;
    }
}


int main() {
    using myNamespace::func;

    func();             // ok
    func(100);      // ok

    return 0;
}
```

